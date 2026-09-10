//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.
#ifndef WEBHOOK_POST_H
#define WEBHOOK_POST_H

#include "webhook_inproc.h"
#include "broker.h"

typedef struct hook_inject_ctx hook_inject_ctx;

extern int hook_entry(nano_work *work, uint8_t reason);
extern int hook_inject_ctx_alloc(conf *nanomq_conf, hook_inject_ctx **ctxp);
extern void hook_inject_ctx_free(hook_inject_ctx *ctx);
extern int hook_publish_from_inject(
    hook_inject_ctx *ctx, nng_socket *hook_sock, nano_work *work);
extern int hook_exchange_init(conf *nanomq_conf, uint64_t num_ctx);
extern int hook_exchange_sender_init(conf *nanomq_conf, struct work **works, uint64_t num_ctx);

extern int webhook_msg_publish(nng_socket *sock, conf_web_hook *hook_conf,
    pub_packet_struct *pub_packet, const char *username,
    const char *client_id);
extern int webhook_client_connack(nng_socket *sock, conf_web_hook *hook_conf,
    uint8_t proto_ver, uint16_t keepalive, uint8_t reason,
    const char *username, const char *client_id);
extern int webhook_client_disconnect(nng_socket *sock,
    conf_web_hook *hook_conf, uint8_t proto_ver, uint16_t keepalive,
    uint8_t reason, const char *username, const char *client_id);

#ifdef SUPP_PARQUET
extern int hook_last_flush();
extern int hook_sync_flush(nng_socket *ex_sock, char *streamid, uint8_t streamtype);
#endif

#endif
