//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "include/webhook_post.h"
#include "include/pub_handler.h"

#include "nng/supplemental/util/platform.h"
#include "nng/supplemental/nanolib/nmq_base64.h"
#include "nng/supplemental/nanolib/cJSON.h"
#include "nng/supplemental/nanolib/canstream.h"
#include "nng/protocol/mqtt/mqtt_parser.h"
#include "nng/supplemental/nanolib/log.h"
#include "nng/exchange/exchange.h"

#ifdef SUPP_PARQUET
#include "nng/supplemental/nanolib/parquet.h"
#endif
#ifdef SUPP_BLF
#include "nng/supplemental/nanolib/blf.h"
#endif

#include "nng/exchange/stream/stream.h"

struct work_cb_arg {
	uint32_t id;
	nng_aio *aio;
	conf    *config;
};

struct hook_inject_ctx {
	struct work_cb_arg *exchange_arg;
	nng_aio           *exchange_aio;
	conf              *config;
};

struct cb_data {
	nng_msg **smsg;
	size_t len;
	struct stream_data_in *sdata;
};

static void cb_data_free(struct cb_data *cb_data);

static bool event_filter(conf_web_hook *hook_conf, webhook_event event);
static bool event_filter_with_topic(
    conf_web_hook *hook_conf, webhook_event event, const char *topic);
static void send_exchange_cb(void *arg);
static int          set_char(char *out, size_t *index,  size_t outlen, char c);
static size_t       base64_no_padding_encode(
    const unsigned char *in, size_t inlen, char *out, size_t outlen);

static size_t       base62_encode(
    const unsigned char *in, size_t inlen, char *out, size_t outlen);

#ifdef SUPP_PARQUET
struct flush_params {
	nng_msg **smsg;
	size_t    len;
	nng_aio * aio;
	char *    topic;
	uint8_t   streamType;
};
static void smsg_free(nng_msg **smsg, size_t len);
static int create_parquet_obj(nng_msg **smsg, size_t len, nng_aio *aio, char *topic, uint8_t streamType, parquet_object **pop);
#endif

#define BASE62_ENCODE_OUT_SIZE(s)                                           \
    ( ((uint64_t)(s) > (UINT64_MAX / 135)) ? 0 :                            \
      (((((uint64_t)(s) * 135) / 100) > ((uint64_t)SIZE_MAX - 4)) ? 0 :     \
      (size_t) (((((uint64_t)(s)) * 135) / 100) + 4)) )

#define BASE64_NO_PADDING_ENCODE_OUT_SIZE(s)                                \
    ( ((uint64_t)(s) > (UINT64_MAX / 8)) ? 0 :                              \
      (((((uint64_t)(s) * 8) / 6) > ((uint64_t)SIZE_MAX - 2)) ? 0 :         \
      (size_t) (((((uint64_t)(s)) * 8) / 6) + 2)) )

static bool
event_filter(conf_web_hook *hook_conf, webhook_event event)
{
	for (uint16_t i = 0; i < hook_conf->rule_count; i++) {
		if (hook_conf->rules[i]->event == event) {
			return true;
		}
	}

	return false;
}

static bool
event_filter_with_topic(
    conf_web_hook *hook_conf, webhook_event event, const char *topic)
{
	for (uint16_t i = 0; i < hook_conf->rule_count; i++) {
		if (hook_conf->rules[i]->event == event) {
			if (hook_conf->rules[i]->topic != NULL) {
				if (!topic_filter(
				        hook_conf->rules[i]->topic, topic)) {
					continue;
				}
			}
			return true;
		}
	}

	return false;
}

static int
set_char(char *out, size_t *index, size_t outlen, char c)
{
	size_t idx = *index;
	if (idx >= outlen - 1) {
		return -1;
	}

	switch (c) {
	case 'i':
		out[idx++] = 'i';
		break;
	case '+':
		out[idx++] = 'A';
		break;
	case '/':
		out[idx++] = 'B';
		break;
	default:
		out[idx++] = c;
		break;
	}
	*index = idx;
	return 0;
}

static size_t
base62_encode(const unsigned char *in, size_t inlen, char *out, size_t outlen)
{
    const char *alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    if (inlen == 0 || out == NULL || outlen == 0) {
        return 0;
    }

    size_t zeros = 0;
    while (zeros < inlen && in[zeros] == 0) {
        zeros++;
    }

    unsigned char *tmp = nng_alloc(inlen);
    if (tmp == NULL) {
        return 0;
    }
    memcpy(tmp, in, inlen);

    size_t out_idx = 0;
    size_t start_idx = zeros;

    while (start_idx < inlen) {
        unsigned int remainder = 0;
        for (size_t i = start_idx; i < inlen; i++) {
            unsigned int dividend = (remainder << 8) | tmp[i];
            tmp[i] = (unsigned char)(dividend / 62);
            remainder = dividend % 62;
        }

        if (out_idx >= outlen - 1) {
            nng_free(tmp, inlen);
            return 0;
        }
        out[out_idx++] = alphabet[remainder];

        while (start_idx < inlen && tmp[start_idx] == 0) {
            start_idx++;
        }
    }

    for (size_t i = 0; i < zeros; i++) {
        if (out_idx >= outlen - 1) {
            nng_free(tmp, inlen);
            return 0;
        }
        out[out_idx++] = alphabet[0];
    }

    for (size_t i = 0; i < out_idx / 2; i++) {
        char t = out[i];
        out[i] = out[out_idx - 1 - i];
        out[out_idx - 1 - i] = t;
    }

    out[out_idx] = '\0';
    nng_free(tmp, inlen);
    return out_idx;
}

static size_t
base64_no_padding_encode(const unsigned char *in, size_t inlen, char *out, size_t outlen)
{
	size_t i, j;
	size_t pos = 0, val = 0;
	const char   base62en[] =
	    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	if (inlen == 0 || out == NULL || outlen == 0) {
		return 0;
	}

	for (i = j = 0; i < inlen; i++) {
		val = (val << 8) | (in[i] & 0xFF);
		pos += 8;
		while (pos > 5) {
			char c = base62en[val >> (pos -= 6)];
			if (set_char(out, &j, outlen, c) != 0)
				return (size_t)-1;
			val &= ((1 << pos) - 1);
		}
	}
	if (pos > 0) {
		char c = base62en[val << (6 - pos)];
		if (set_char(out, &j, outlen, c) != 0)
			return (size_t)-1;
	}
	if (j < outlen) {
		out[j] = '\0';
	}
	return j;
}

int
webhook_msg_publish(nng_socket *sock, conf_web_hook *hook_conf,
    pub_packet_struct *pub_packet, const char *username, const char *client_id)
{
	if (!hook_conf->enable ||
	    !event_filter_with_topic(hook_conf, MESSAGE_PUBLISH,
	        pub_packet->var_header.publish.topic_name.body)) {
		return -1;
	}

	cJSON *obj = cJSON_CreateObject();

	cJSON_AddNumberToObject(obj, "ts", nng_timestamp());
	cJSON_AddStringToObject(
	    obj, "topic", pub_packet->var_header.publish.topic_name.body);
	cJSON_AddBoolToObject(obj, "retain", pub_packet->fixed_header.retain);
	cJSON_AddNumberToObject(obj, "qos", pub_packet->fixed_header.qos);
	cJSON_AddStringToObject(obj, "action", "message_publish");
	cJSON_AddStringToObject(
	    obj, "from_username", username == NULL ? "undefined" : username);
	if (client_id) {
		cJSON_AddStringToObject(obj, "from_client_id", client_id);
	} else {
		cJSON_AddNullToObject(obj, "from_client_id");
	}
	size_t out_size = 0;
	char  *encode   = NULL;
	size_t len      = 0;
	switch (hook_conf->encode_payload) {
	case plain:
		cJSON_AddStringToObject(
		    obj, "payload", (const char *) pub_packet->payload.data);
		break;
	case base64:
		out_size = BASE64_ENCODE_OUT_SIZE((uint64_t)pub_packet->payload.len);
		if (out_size == 0) {
				log_error("Payload is too large for Base64 encoding.");
				len = 0;
		} else {
			encode = nng_zalloc(out_size);
			len    = nmq_base64_encode(
				(const uint8_t *) pub_packet->payload.data,
				pub_packet->payload.len, encode, out_size);
		}
		if (len != (size_t)-1 && len > 0) {
			cJSON_AddStringToObject(obj, "payload", encode);
		} else {
			cJSON_AddNullToObject(obj, "payload");
		}
		nng_strfree(encode);
		break;
	case base64_no_padding:
		out_size = BASE64_NO_PADDING_ENCODE_OUT_SIZE(pub_packet->payload.len);
		if (out_size == 0) {
			log_error("Payload is too large for Base64 encoding.");
			len = 0;
		} else {
			encode   = nng_zalloc(out_size);
			len      = base64_no_padding_encode(
					pub_packet->payload.data, pub_packet->payload.len, encode, out_size);
		}
		if (len != (size_t)-1 && len > 0) {
			cJSON_AddStringToObject(obj, "payload", encode);
		} else {
			cJSON_AddNullToObject(obj, "payload");
		}
		nng_strfree(encode);
		break;
	case base62:
		out_size = BASE62_ENCODE_OUT_SIZE(pub_packet->payload.len);
		if (out_size == 0) {
			log_error("Payload is too large for Base62 encoding.");
			len = 0;
		} else {
			encode   = nng_zalloc(out_size);
			len      = base62_encode(
                 pub_packet->payload.data, pub_packet->payload.len, encode, out_size);
		}
        if (len != (size_t)-1 && len > 0) {
            cJSON_AddStringToObject(obj, "payload", encode);
        } else {
            // Handle empty payload or alloc failure
            if (pub_packet->payload.len == 0) {
                 cJSON_AddStringToObject(obj, "payload", "");
            } else {
                 cJSON_AddNullToObject(obj, "payload");
            }
        }
        nng_strfree(encode);
        break;

	default:
		break;
	}

	char *json = cJSON_PrintUnformatted(obj);

	int rv = nng_send(*sock, json, strlen(json), NNG_FLAG_NONBLOCK);
	if (rv != 0)
		log_error("nng_send failed %d %s", rv, nng_strerror(rv));

	nng_strfree(json);
	cJSON_Delete(obj);

	return rv;
}

int
webhook_client_connack(nng_socket *sock, conf_web_hook *hook_conf,
    uint8_t proto_ver, uint16_t keepalive, uint8_t reason,
    const char *username, const char *client_id)
{
	if (!hook_conf->enable || !event_filter(hook_conf, CLIENT_CONNACK)) {
		log_info("event is filterd out!");
		return -1;
	}

	cJSON *obj = cJSON_CreateObject();

	cJSON_AddNumberToObject(obj, "proto_ver", proto_ver);
	cJSON_AddNumberToObject(obj, "keepalive", keepalive);
	// TODO get reason string
	cJSON_AddStringToObject(
	    obj, "conn_ack", reason == SUCCESS ? "success" : "fail");
	cJSON_AddStringToObject(
	    obj, "username", username == NULL ? "undefined" : username);
	cJSON_AddStringToObject(obj, "clientid", client_id);
	cJSON_AddStringToObject(obj, "action", "client_connack");

	char *json = cJSON_PrintUnformatted(obj);

	int rv = nng_send(*sock, json, strlen(json), NNG_FLAG_NONBLOCK);
	if (rv != 0)
		log_error("nng_send failed %d %s", rv, nng_strerror(rv));

	nng_strfree(json);
	cJSON_Delete(obj);

	return rv;
}

int
webhook_client_disconnect(nng_socket *sock, conf_web_hook *hook_conf,
    uint8_t proto_ver, uint16_t keepalive, uint8_t reason,
    const char *username, const char *client_id)
{
	if (!hook_conf->enable ||
	    !event_filter(hook_conf, CLIENT_DISCONNECTED)) {
		return -1;
	}

	cJSON *obj = cJSON_CreateObject();
	// TODO get reason string
	cJSON_AddStringToObject(
	    obj, "reason", reason == SUCCESS ? "normal" : "abnormal");
	cJSON_AddStringToObject(
	    obj, "username", username == NULL ? "undefined" : username);
	cJSON_AddStringToObject(obj, "clientid", client_id);
	cJSON_AddStringToObject(obj, "action", "client_disconnected");

	char *json = cJSON_PrintUnformatted(obj);

	int rv = nng_send(*sock, json, strlen(json), NNG_FLAG_NONBLOCK);
	if (rv != 0)
		log_error("nng_send failed %d %s", rv, nng_strerror(rv));

	nng_strfree(json);
	cJSON_Delete(obj);

	return rv;
}

static inline uint64_t
gen_hash_nearby_key(char *clientid, char *topic, uint32_t pid)
{
	uint64_t res = 0;
	char buf[11]; // should be enough put a u32
	sprintf(buf, "%d", pid);

	char buf2[100]; // should be enough put the clienid and topic and pid
	sprintf(buf2, "%s%s%s", clientid, topic, buf);

	uint32_t key32 = DJBHash(buf2);
	res = key32; res = res << 32; res += key32;
	// log_info("%s - %s - %s => %lx %llx", clientid, topic, buf, key32, res);
	// nng_time ts = nng_timestamp();
	return res;
}

static uint32_t g_inc_id = 0;
#ifdef SUPP_PARQUET
static conf *tmp_root_conf = NULL;
#endif
static nng_mtx *ts_mtx;
static nng_time ts_last;

static int
dup_publish_msg_for_exchange(nano_work *work, nng_msg **out)
{
	if (work == NULL || work->msg == NULL || out == NULL) {
		return NNG_EINVAL;
	}

	nng_msg *msg = NULL;
	int rv = nng_msg_alloc(&msg, 0);
	if (rv != 0) {
		return rv;
	}
	rv = nng_msg_header_append(
	    msg, nng_msg_header(work->msg), nng_msg_header_len(work->msg));
	if (rv == 0) {
		rv = nng_msg_append(msg, nng_msg_body(work->msg), nng_msg_len(work->msg));
	}
	if (rv != 0) {
		nng_msg_free(msg);
		return rv;
	}

	uint8_t *body_ptr = nng_msg_body(work->msg);
	uint8_t *payload_ptr = nng_msg_payload_ptr(work->msg);
	ptrdiff_t offset = (payload_ptr != NULL && body_ptr != NULL) ?
	    (ptrdiff_t)(payload_ptr - body_ptr) : 0;
	nng_msg_set_payload_ptr(msg, (uint8_t *) nng_msg_body(msg) + offset);
	*out = msg;
	return 0;
}

static nng_time
next_monotonic_exchange_ts(void)
{
	nng_time ts = nng_timestamp();
	if (ts_mtx != NULL) {
		nng_mtx_lock(ts_mtx);
		if (ts <= ts_last) {
			if (ts_last - ts <= 1000) {
				ts = ts_last + 1;
			}
		}
		ts_last = ts;
		nng_mtx_unlock(ts_mtx);
	}
	return ts;
}

static int
send_publish_to_exchange(nano_work *work, nng_aio *aio, nng_time ts)
{
	if (work == NULL || work->config == NULL || work->pub_packet == NULL ||
	    aio == NULL) {
		return NNG_EINVAL;
	}

	conf_exchange *ex_conf = &work->config->exchange;
	nng_msg *msg = NULL;
	int rv = dup_publish_msg_for_exchange(work, &msg);
	if (rv != 0) {
		return rv;
	}
	nng_msg_set_timestamp(msg, ts);

	for (size_t i = 0; i < ex_conf->count; i++) {
		if (topic_filter(ex_conf->nodes[i]->topic,
		        work->pub_packet->var_header.publish.topic_name.body)) {
			nng_aio_wait(aio);
			nng_msg_clone(msg);
			nng_aio_set_msg(aio, msg);
			nng_send_aio(*ex_conf->nodes[i]->sock, aio);
			break;
		}
	}

	nng_msg_free(msg);
	return 0;
}

int
hook_inject_ctx_alloc(conf *nanomq_conf, hook_inject_ctx **ctxp)
{
	if (nanomq_conf == NULL || ctxp == NULL) {
		return NNG_EINVAL;
	}

	hook_inject_ctx *ctx = nng_zalloc(sizeof(*ctx));
	if (ctx == NULL) {
		return NNG_ENOMEM;
	}
	ctx->config = nanomq_conf;

	if (nanomq_conf->exchange.count > 0) {
		ctx->exchange_arg = nng_zalloc(sizeof(*ctx->exchange_arg));
		if (ctx->exchange_arg == NULL) {
			nng_free(ctx, sizeof(*ctx));
			return NNG_ENOMEM;
		}
		ctx->exchange_arg->id = UINT32_MAX;
		ctx->exchange_arg->config = nanomq_conf;
		int rv = nng_aio_alloc(&ctx->exchange_aio, send_exchange_cb,
		    ctx->exchange_arg);
		if (rv != 0) {
			nng_free(ctx->exchange_arg, sizeof(*ctx->exchange_arg));
			nng_free(ctx, sizeof(*ctx));
			return rv;
		}
		ctx->exchange_arg->aio = ctx->exchange_aio;
	}

	*ctxp = ctx;
	return 0;
}

void
hook_inject_ctx_free(hook_inject_ctx *ctx)
{
	if (ctx == NULL) {
		return;
	}
	if (ctx->exchange_aio != NULL) {
		nng_aio_wait(ctx->exchange_aio);
		nng_aio_free(ctx->exchange_aio);
		ctx->exchange_aio = NULL;
	}
	if (ctx->exchange_arg != NULL) {
		nng_free(ctx->exchange_arg, sizeof(*ctx->exchange_arg));
		ctx->exchange_arg = NULL;
	}
	nng_free(ctx, sizeof(*ctx));
}

int
hook_publish_from_inject(hook_inject_ctx *ctx, nng_socket *hook_sock, nano_work *work)
{
	if (ctx == NULL || work == NULL || work->config == NULL ||
	    work->pub_packet == NULL || work->msg == NULL) {
		return NNG_EINVAL;
	}

	int rv = 0;
	conf_web_hook *hook_conf = &work->config->web_hook;
	conf_exchange *ex_conf = &work->config->exchange;
	conf_parquet *parquetconf = &work->config->parquet;
	conf_blf *blfconf = &work->config->blf;

#ifdef SUPP_PARQUET
	if (!tmp_root_conf) {
		tmp_root_conf = work->config;
	}
#endif

	if (work->flag == CMD_PUBLISH &&
	    nng_msg_get_type(work->msg) == CMD_PUBLISH &&
	    ex_conf->count > 0 && ctx->exchange_aio != NULL) {
		if (parquetconf->enable == true) {
			rv = send_publish_to_exchange(
			    work, ctx->exchange_aio, next_monotonic_exchange_ts());
		}
		if (rv == 0 && blfconf->enable == true) {
			char *clientid = (char *) conn_param_get_clientid(work->cparam);
			char *topic = work->pub_packet->var_header.publish.topic_name.body;
			if (clientid != NULL && topic != NULL) {
				nng_mtx_lock(hook_conf->ex_mtx);
				uint32_t pid = g_inc_id++;
				nng_mtx_unlock(hook_conf->ex_mtx);
				rv = send_publish_to_exchange(work, ctx->exchange_aio,
				    (nng_time) gen_hash_nearby_key(clientid, topic, pid));
			}
		}
	}

	if (hook_conf->enable && hook_sock != NULL &&
	    work->flag == CMD_PUBLISH) {
		rv = webhook_msg_publish(hook_sock, hook_conf, work->pub_packet,
		    (const char *) conn_param_get_username(work->cparam),
		    (const char *) conn_param_get_clientid(work->cparam));
	}

	work->flag = 0;
	return rv;
}

#ifdef SUPP_PARQUET
int
hook_sync_flush(nng_socket *ex_sock, char *streamid, uint8_t streamtype)
{
	nng_msg *m;
	nng_msg_alloc(&m, 0);
	nng_aio *aio;
	nng_aio_alloc(&aio, NULL, NULL);
	nng_aio *faio;
	nng_aio_alloc(&faio, NULL, NULL);
	if (!m || !aio || !faio) {
		log_error("Error in alloc memory");
		return -1;
	}

	int       rv;
	int       rc  = 0;
	nng_time *tss = NULL;
	tss = nng_alloc(sizeof(nng_time) * 3);
	tss[0] = 0;
	tss[1] = 9223372036854775807; // big enough
	tss[2] = 1; // It's a clean flag
	nng_msg_set_proto_data(m, NULL, (void *)tss);
	nng_aio_set_msg(aio, m);

	// Do clean on MQ and get the returned msgs
	nng_recv_aio(*ex_sock, aio);
	nng_aio_wait(aio);
	if ((rc = nng_aio_result(aio)) != 0) {
		log_warn("error%d in getting msgs from exchange(%s)", rc, streamid);
		rc = -2;
		goto done;
	}

	nng_msg **msgs_res = (nng_msg **)nng_aio_get_msg(aio);
	uint32_t  msgs_len = (uintptr_t)nng_aio_get_prov_data(aio);
	log_info("Clean exchange(%s) and get %dmsgs type%d", streamid, msgs_len, streamtype);

	parquet_object *po = NULL;
	if (msgs_len > 0 && msgs_res != NULL) {
		// we don't care the result of last flush
		rv = create_parquet_obj(msgs_res, msgs_len, faio, streamid, streamtype, &po);
		if (rv != 0) {
			log_error("error%d in put msgs in exchange(%s) to parquet", rv, streamid);
			if (rv != NNG_EINVAL)
				smsg_free(msgs_res, msgs_len);
		} else {
			nng_aio_begin(faio);
			parquet_write_batch_async(po);
			nng_aio_wait(faio);

			if ((rv = nng_aio_result(faio)) != 0) {
				log_warn("error%d in flush msgs in exchange(%s) to parquet",
						rv, streamid);
			}
		}

		struct cb_data *cb_data = (struct cb_data *)nng_aio_get_prov_data(faio);
		if (cb_data == NULL) {
			log_error("cb_data is NULL");
		} else {
			smsg_free(cb_data->smsg, cb_data->len);
			cb_data_free(cb_data);
		}

		if (rv != 0) {
			rc = -1;
			goto done;
		}
		log_warn("flush %dmsgs in exchange(%s) to parquet done!", msgs_len, streamid);
	} else {
		rc = -2;
		goto done;
	}

done:
	nng_aio_free(faio);
	nng_aio_free(aio);
	nng_msg_free(m);
	nng_free(tss, 0);
	return rc;
}

int
hook_last_flush()
{
	if (!tmp_root_conf)
		return -1;
	int            rv;
	nng_socket    *ex_sock;
	conf_exchange *exconf = &tmp_root_conf->exchange;
	if (exconf->count == 0) {
		return -2;
	}

	nng_aio *aio;
	nng_aio_alloc(&aio, NULL, NULL);
	nng_aio *faio;
	nng_aio_alloc(&faio, NULL, NULL);
	for (int i=0; i<exconf->count; ++i) {
		ex_sock = exconf->nodes[i]->sock;
		if (!ex_sock) {
			log_error("exchange(%s) socket is null.", exconf->nodes[i]->name);
			continue;
		}
		char *  streamid   = exconf->nodes[i]->topic;
		uint8_t streamtype = exconf->nodes[i]->streamType;
		hook_sync_flush(ex_sock, streamid, streamtype);
	}
	nng_aio_free(aio);
	nng_aio_free(faio);
	return 0;
}
#endif

inline int
hook_entry(nano_work *work, uint8_t reason)
{
	int            rv        = 0;
	conf_web_hook *hook_conf = &work->config->web_hook;
	conf_exchange *ex_conf   = &work->config->exchange;
	conn_param    *cparam    = work->cparam;
	nng_socket    *sock      = &work->hook_sock;
	nng_socket    *ex_sock;
	conf_parquet  *parquetconf = &work->config->parquet;
	conf_blf      *blfconf     = &work->config->blf;

#ifdef SUPP_PARQUET
	// Just for hook_last_flush. Plz don't use it in other cases.
	// Not Thread safe... will be eliminated in master branch
	if (!tmp_root_conf)
		tmp_root_conf = work->config;
#endif
	// process MQ msg first, only pub msg is valid
	// discard online/offline event msg?
	if (ex_conf->count > 0 && parquetconf->enable == true
		&& (work->flag == CMD_PUBLISH)
		&& nng_msg_get_type(work->msg) == CMD_PUBLISH) {
		// dup msg for now, TODO or reuse it?
		nng_msg *msg;
		nng_msg_alloc(&msg, 0);
		nng_msg_header_append(msg, nng_msg_header(work->msg), nng_msg_header_len(work->msg));
		nng_msg_append(msg, nng_msg_body(work->msg), nng_msg_len(work->msg));

		uint8_t *body_ptr = nng_msg_body(work->msg);
		ptrdiff_t offset = (ptrdiff_t)(nng_msg_payload_ptr(work->msg) - body_ptr);
		nng_msg_set_payload_ptr(msg, (uint8_t *)nng_msg_body(msg) + offset);

		/*
		// cparam has cloned at outside of hook_entry
		char *clientid = (char *)conn_param_get_clientid(work->cparam);
		if (clientid == NULL)
			goto done;
		char *topic = work->pub_packet->var_header.publish.topic_name.body;
		if (topic == NULL)
			goto done;
		nng_mtx_lock(hook_conf->ex_mtx);
		uint32_t pid = g_inc_id ++;
		nng_mtx_unlock(hook_conf->ex_mtx);

		nng_time ts = (nng_time)gen_hash_nearby_key(clientid, topic, pid);
		*/

		nng_time ts = nng_timestamp();
		if (ts_mtx != NULL) {
			nng_mtx_lock(ts_mtx);
			if (ts <= ts_last) {
				if (ts_last - ts > 1000) {
					// log_debug("Timestamp lag over 1s");
					// do nothing and exchange will handle this msg properly
				} else {
					ts = ts_last + 1;
				}
			}
			ts_last = ts;
			nng_mtx_unlock(ts_mtx);
		}
		nng_msg_set_timestamp(msg, ts);
		size_t hook_ctx = work->config->web_hook.pool_size;
		for (size_t i = 0; i < ex_conf->count; i++) {
			if (topic_filter(ex_conf->nodes[i]->topic,
			        work->pub_packet->var_header.publish.topic_name.body)) {

				// msg from bridge or HTTP is also allowed here
				// Need to deduct Hook CTX, because it was init before broker CTX
				nng_aio *aio = hook_conf->saios[work->work_id];
				nng_aio_wait(aio);

				nng_msg_clone(msg);
				nng_aio_set_msg(aio, msg);

				ex_sock = ex_conf->nodes[i]->sock;
				nng_send_aio(*ex_sock, aio);
				break;
			}
		}
		nng_msg_free(msg); // Cloned for each exchange before
	}

	if (ex_conf->count > 0 && blfconf->enable == true
		&& (work->flag == CMD_PUBLISH)
		&& nng_msg_get_type(work->msg) == CMD_PUBLISH) {
		// dup msg for now, TODO or reuse it?
		nng_msg *msg;
		nng_msg_alloc(&msg, 0);
		nng_msg_header_append(msg, nng_msg_header(work->msg), nng_msg_header_len(work->msg));
		nng_msg_append(msg, nng_msg_body(work->msg), nng_msg_len(work->msg));

		uint8_t *body_ptr = nng_msg_body(work->msg);
		ptrdiff_t offset = (ptrdiff_t)(nng_msg_payload_ptr(work->msg) - body_ptr);
		nng_msg_set_payload_ptr(msg, (uint8_t *)nng_msg_body(msg) + offset);

		// cparam has cloned at outside of hook_entry
		char *clientid = (char *)conn_param_get_clientid(work->cparam);
		if (clientid == NULL)
			goto done;
		char *topic = work->pub_packet->var_header.publish.topic_name.body;
		if (topic == NULL)
			goto done;
		nng_mtx_lock(hook_conf->ex_mtx);
		uint32_t pid = g_inc_id ++;
		nng_mtx_unlock(hook_conf->ex_mtx);

		nng_time ts = (nng_time)gen_hash_nearby_key(clientid, topic, pid);
		nng_msg_set_timestamp(msg, ts);

		for (size_t i = 0; i < ex_conf->count; i++) {
			if (topic_filter(ex_conf->nodes[i]->topic,
			        work->pub_packet->var_header.publish.topic_name.body)) {

				if (work->ctx.id > work->config->parallel)
					log_error("parallel %d idx %d", work->config->parallel);	// shall be a bug if triggered

				nng_aio *aio = hook_conf->saios[work->ctx.id - 1];
				nng_aio_wait(aio);

				nng_msg_clone(msg);
				nng_aio_set_msg(aio, msg);

				ex_sock = ex_conf->nodes[i]->sock;
				nng_send_aio(*ex_sock, aio);
				break;
			}
		}
		nng_msg_free(msg); // Cloned for each exchange before
	}


	if (!hook_conf->enable) {
		log_debug("webhook is disabled!");
		return 0;
	}
	switch (work->flag) {
	case CMD_CONNACK:
		rv = webhook_client_connack(sock, hook_conf,
		    conn_param_get_protover(cparam),
		    conn_param_get_keepalive(cparam), reason,
		    (const char*)conn_param_get_username(cparam),
		    (const char*)conn_param_get_clientid(cparam));
		break;
	case CMD_PUBLISH:
		rv = webhook_msg_publish(sock, hook_conf, work->pub_packet,
		    (const char*)conn_param_get_username(cparam),
		    (const char*)conn_param_get_clientid(cparam));
		break;
	case CMD_DISCONNECT_EV:
		rv = webhook_client_disconnect(sock, hook_conf,
		    conn_param_get_protover(cparam),
		    conn_param_get_keepalive(cparam), reason,
		    (const char*)conn_param_get_username(cparam),
		    (const char*)conn_param_get_clientid(cparam));
	case CMD_SUBSCRIBE:
		break;
	case CMD_UNSUBSCRIBE:
		break;
	default:
		break;
	}

done:
	// Do not let online event msg trigger webhook
	work->flag = 0;
	return rv;
}

static struct stream_data_in *stream_data_in_init(size_t len, nng_msg **smsg)
{
	struct stream_data_in *sdata = nng_alloc(sizeof(struct stream_data_in));
	if (sdata == NULL) {
		return NULL;
	}

	sdata->datas = nng_alloc(sizeof(void *) * len);
	if (sdata->datas == NULL) {
		nng_free(sdata, sizeof(struct stream_data_in));
		return NULL;
	}

	sdata->keys = nng_alloc(sizeof(uint64_t) * len);
	if (sdata->keys == NULL) {
		nng_free(sdata->datas, sizeof(void *) * len);
		nng_free(sdata, sizeof(struct stream_data_in));
		return NULL;
	}

	sdata->lens = nng_alloc(sizeof(uint32_t) * len);
	if (sdata->lens == NULL) {
		nng_free(sdata->datas, sizeof(void *) * len);
		nng_free(sdata->keys, sizeof(uint64_t) * len);
		nng_free(sdata, sizeof(struct stream_data_in));
		return NULL;
	}

	size_t len2 = 0;
	for (size_t i = 0; i < len; i++) {
		if (smsg[i] == NULL) {
			continue;
		}
		sdata->datas[len2] = nng_msg_payload_ptr(smsg[i]);
		sdata->lens[len2] = nng_msg_len(smsg[i]) - (nng_msg_payload_ptr(smsg[i]) - (uint8_t *)nng_msg_body(smsg[i]));
		sdata->keys[len2] = nng_msg_get_timestamp(smsg[i]);
		len2++;
	}

	sdata->len = len2;

	return sdata;
}

static struct cb_data *cb_data_init(nng_msg **smsg, size_t len)
{
	struct cb_data *cb_data = nng_alloc(sizeof(struct cb_data));
	if (cb_data == NULL) {
		return NULL;
	}

	cb_data->smsg = smsg;
	cb_data->len = len;
	cb_data->sdata = stream_data_in_init(len, smsg);
	if (cb_data->sdata == NULL) {
		nng_free(cb_data, sizeof(struct cb_data));
		return NULL;
	}

	return cb_data;
}

static void smsg_free(nng_msg **smsg, size_t len)
{
	for (size_t i = 0; i < len; i++) {
		if (smsg[i] != NULL) {
			nng_msg_free(smsg[i]);
		}
	}

	nng_free(smsg, len);

	return;
}

static void cb_data_free(struct cb_data *cb_data)
{
	if (cb_data == NULL) {
		return;
	}

	if (cb_data->sdata != NULL) {
		stream_data_in_free(cb_data->sdata);
	}

	nng_free(cb_data, sizeof(struct cb_data));
	return;
}

#ifdef SUPP_PARQUET
static int
create_parquet_obj(nng_msg **smsg, size_t len, nng_aio *aio, char *topic, uint8_t streamType, parquet_object **pop)
{
	struct cb_data *cb_data = NULL;
	void *encoded_stream_data = NULL;

	cb_data = cb_data_init(smsg, len);
	if (cb_data == NULL) {
		log_error("cb_data_init failed");
		return NNG_ENOMEM;
	}

	encoded_stream_data = stream_encode(streamType, cb_data->sdata);
	if (encoded_stream_data == NULL) {
		log_error("encode encoded_stream_data failed");
		smsg_free(cb_data->smsg, cb_data->len);
		cb_data_free(cb_data);
		return NNG_EINVAL;
	}

	parquet_object *parquet_obj = NULL;
	parquet_obj = parquet_object_alloc(encoded_stream_data, WRITE_RAW, aio, (void *)cb_data, topic);
	if (parquet_obj == NULL) {
		log_error("parquet_object_alloc failed");
		parquet_data_free(encoded_stream_data);
		cb_data_free(cb_data);
		return NNG_ENOMEM;
	}

	*pop = parquet_obj;
	return 0;
}
#endif

static int inline get_flush_params(nng_aio *aio,
								   nng_msg *msg,
								   nng_msg ***pmsgs_del,
								   int **pmsgs_lenp,
								   char **ptopic,
								   uint8_t *pstreamType)
{
	uint8_t streamType = 0;
	char *topic = NULL;
	int *msgs_len = NULL;
	nng_msg **msgs_del = NULL;

	msgs_del = nng_aio_get_prov_data(aio);
	msgs_len = (int *)nng_msg_get_proto_data(msg);
	topic = nng_msg_get_conn_param(msg);
	streamType = nng_msg_get_cmd_type(msg);

	if (msgs_len == NULL || msgs_del == NULL || topic == NULL) {
		if (msgs_del != NULL) {
			if (msgs_len != NULL) {
				for (int i = 0; i < *msgs_len; ++i) {
					if (msgs_del[i] != NULL) {
						nng_msg_free(msgs_del[i]);
					}
				}
			}
			nng_free(msgs_del, sizeof(nng_msg *));
		}
		if (msgs_len != NULL) {
			nng_free(msgs_len, sizeof(int));
		}
		if (topic != NULL) {
			nng_free(topic, strlen(topic) + 1);
		}
		return NNG_EINVAL;
	}

	*ptopic = topic;
	*pmsgs_del = msgs_del;
	*pmsgs_lenp = msgs_len;
	*pstreamType = streamType;

	return 0;
}

static void
send_exchange_cb(void *arg)
{
	int rv;
	char *topic = NULL;
	int *msgs_lenp = NULL;
	nng_aio *aio = NULL;
	nng_msg *msg = NULL;
	nng_msg **msgs_del = NULL;
	struct work_cb_arg *w_cb_arg = NULL;
	uint8_t streamType = 0;
	conf  *config;
	conf_web_hook *hook_conf = NULL;

	w_cb_arg = arg;
	config = w_cb_arg->config;
	hook_conf = &config->web_hook;

	aio = w_cb_arg->aio;
	if ((rv = nng_aio_result(aio)) != 0) {
		log_error("error %d in send to exchange", rv);
		return;
	}

	msg = nng_aio_get_msg(aio);
	if (msg == NULL) {
		return;
	}

#ifndef SUPP_PARQUET
	log_error("ENABLE_PARQUET is not defined flush to disk failed");
	nng_msg_free(msg);
	return;
#endif

	rv = get_flush_params(aio, msg, &msgs_del, &msgs_lenp, &topic, &streamType);
	if (rv != 0) {
		log_error("get_flush_params error %d", rv);
		nng_msg_free(msg);
		return;
	}

	// Flush to disk.
	if (config->parquet.enable || config->blf.enable) {
		nng_mtx_lock(hook_conf->ex_mtx);
#ifdef SUPP_PARQUET
		parquet_object *po = NULL;
		if (hook_conf->ex_busy) {
			rv = NNG_EBUSY;
		} else {
			rv = create_parquet_obj(
				msgs_del, *msgs_lenp, hook_conf->ex_aio, topic, streamType, &po);
		}
		if ((rv == NNG_EBUSY || rv == NNG_ENOMEM) && hook_conf->ex_lmq_size > 0) {
			struct flush_params *fp = nng_alloc(sizeof(struct flush_params));
			if (!fp) {
				smsg_free(msgs_del, *msgs_lenp);
				log_error("failed to alloc flush_params struct");
			} else {
				fp->smsg = msgs_del;
				fp->len = *msgs_lenp;
				fp->aio = hook_conf->ex_aio;
				fp->topic = topic;
				fp->streamType = streamType;
				if (0 != nng_lmq_put(hook_conf->ex_lmq, (nng_msg *)fp)) {
					smsg_free(msgs_del, *msgs_lenp);
					nng_free(fp, 0);
					log_error("flush error %d, and cache smsg failed too", rv);
				} else {
					log_warn("flush error %d, but cached", rv);
				}
			}
		} else if (rv == NNG_EBUSY || rv == NNG_ENOMEM) {
			smsg_free(msgs_del, *msgs_lenp);
			log_warn("flush error %d, no lmq cache, discarding", rv);
		} else if (rv == NNG_EINVAL) { // Configuration error. should not retry
			log_error("flush error %d, configuration error, no cache", rv);
		} else if (rv == 0) {
			hook_conf->ex_busy = true;
			parquet_write_batch_async(po);
		}
#endif
		nng_mtx_unlock(hook_conf->ex_mtx);
	} else {
		smsg_free(msgs_del, *msgs_lenp);
	}

	nng_msg_free(msg);
	nng_free(msgs_lenp, sizeof(int));
	return;
}

// Better to be done in sync
static void
send_parquet_cb(void *arg)
{
#ifdef SUPP_PARQUET
	int rv;
	conf_web_hook *hook_conf = arg;
	nng_aio *aio = hook_conf->ex_aio;
	parquet_object *po = NULL;

	nng_mtx_lock(hook_conf->ex_mtx);

	struct cb_data *cb_data = (struct cb_data *)nng_aio_get_prov_data(aio);
	if (cb_data == NULL) {
		hook_conf->ex_busy = false;
		nng_mtx_unlock(hook_conf->ex_mtx);
		log_error("cb_data is NULL");
		return;
	}

	smsg_free(cb_data->smsg, cb_data->len);
	cb_data_free(cb_data);

	if (nng_aio_result(aio) != 0) {
		hook_conf->ex_busy = false;
		nng_mtx_unlock(hook_conf->ex_mtx);
		log_warn("Write data to parquet failed");
		return;
	}

	if (hook_conf->ex_lmq_size > 0 && !nng_lmq_empty(hook_conf->ex_lmq)) {
		struct flush_params *fp = NULL;
		if (0 != nng_lmq_get(hook_conf->ex_lmq, (nng_msg **)&fp)) {
			log_error("flush lmq is not empty. but get failed.");
			hook_conf->ex_busy = false;
			nng_mtx_unlock(hook_conf->ex_mtx);
			return;
		}
		rv = create_parquet_obj(fp->smsg, fp->len, fp->aio, fp->topic, fp->streamType, &po);
		if (rv != 0) {
		// XXX we may need to notice a problem that the order of flush is changed
			if (0 != nng_lmq_put(hook_conf->ex_lmq, (nng_msg *)fp)) {
				smsg_free(fp->smsg, fp->len);
				nng_free(fp, 0);
				log_error("flush error %d, and cache smsg failed too", rv);
			} else {
				log_warn("flush error %d, but cached", rv);
			}
			hook_conf->ex_busy = false;
		} else {
			// hook_conf->ex_busy = true; it was true
			parquet_write_batch_async(po);
			nng_free(fp, 0);
		}
	} else {
		hook_conf->ex_busy = false;
	}
	nng_mtx_unlock(hook_conf->ex_mtx);
#endif

	return;
}

int
hook_exchange_init(conf *nanomq_conf, uint64_t num_ctx)
{
	conf_web_hook *hook_conf = &nanomq_conf->web_hook;

	nng_mtx_alloc(&hook_conf->ex_mtx);
	nng_aio_alloc(&hook_conf->ex_aio, send_parquet_cb, hook_conf);
	hook_conf->ex_busy = false;
	if (hook_conf->ex_lmq_size > 16)
		log_warn("capacity of cache lmq for flush parquet is so large that may cost too much memory.");
	if (hook_conf->ex_lmq_size > 0) {
		if (0 != nng_lmq_alloc(&hook_conf->ex_lmq, hook_conf->ex_lmq_size)) {
			log_error("failed to alloc cache lmq for flush parquet. reset to 0");
			hook_conf->ex_lmq_size = 0; // reset
		}
	}
	hook_conf->saios = nng_alloc(sizeof(nng_aio *) * num_ctx);

	if (0 != nng_mtx_alloc(&ts_mtx)) {
		log_error("Failed to alloc ts mtx");
	}
	ts_last = 0;

	return 0;
}

int
hook_exchange_sender_init(conf *nanomq_conf, struct work **works, uint64_t num_ctx)
{
	conf_web_hook *hook_conf = &nanomq_conf->web_hook;
	conf_exchange *exchange_conf = &nanomq_conf->exchange;
	conf_blf *blf_conf = &nanomq_conf->blf;

	for (int i = 0; i < num_ctx; ++i) {
		struct work_cb_arg *w_cb_arg = NULL;
		w_cb_arg = nng_alloc(sizeof(struct work_cb_arg));
		if (w_cb_arg == NULL) {
			log_error("nng_alloc failed");
			return NNG_ENOMEM;
		}

		nng_aio_alloc(
		    &hook_conf->saios[i], send_exchange_cb, w_cb_arg);
		w_cb_arg->id = i;
		w_cb_arg->aio = hook_conf->saios[i];
		w_cb_arg->config = nanomq_conf;
	}

#ifdef SUPP_PARQUET
	log_info("init parquet_write_launcher");
	parquet_write_launcher(exchange_conf);
#endif

#ifdef SUPP_BLF
	if (blf_conf->enable) {
		log_info("init blf_write_launcher");
		blf_write_launcher(blf_conf);
	}
#endif

	return 0;
}

