//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "nano_sdk.h"     // Broker-facing API used in runtime integration.
#include "nano_skill.h"   // Standalone helpers: window/batch/time/dbc.

#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#define METRICS_WINDOW_MS 5000

typedef int32_t nng_duration;
typedef struct nng_mtx nng_mtx;
typedef struct nng_thread nng_thread;

extern int  nng_mtx_alloc(nng_mtx **);
extern void nng_mtx_free(nng_mtx *);
extern void nng_mtx_lock(nng_mtx *);
extern void nng_mtx_unlock(nng_mtx *);
extern int  nng_thread_create(nng_thread **, void (*)(void *), void *);
extern void nng_thread_destroy(nng_thread *);
extern void nng_msleep(nng_duration);

static nano_skill_window *g_win;
static nano_skill_batch  *g_batch;
static nng_mtx           *g_win_mtx;
static nng_thread        *g_timer_thr;
static bool               g_timer_stop;

static void
publish_metrics_line(uint64_t ts_ms, uint32_t count, double avg)
{
	char buf[200];
	int  n = snprintf(buf, sizeof(buf),
	    "{\"ts\":%" PRIu64 ",\"count\":%u,\"avg\":%.2f}",
	    ts_ms, count, avg);
	if (n > 0) {
		int rv = nano_mqtt_publish("metrics/example", buf, (uint32_t) n, 0, false);
		if (rv == -ENOSYS) {
			(void) nano_file_append("/tmp/stream_plugin_metrics.jsonl", buf, (uint32_t) n);
			(void) nano_file_append("/tmp/stream_plugin_metrics.jsonl", "\n", 1);
		}
	}
}

static void
metrics_timer(void *arg)
{
	(void) arg;

	while (!g_timer_stop) {
		for (uint32_t elapsed = 0; elapsed < METRICS_WINDOW_MS && !g_timer_stop; elapsed += 100) {
			nng_msleep(100);
		}
		if (g_timer_stop) {
			break;
		}

		uint32_t count = 0;
		double   avg   = 0.0;
		uint64_t ts_ms = nano_time_ms();

		if (g_win_mtx) nng_mtx_lock(g_win_mtx);
		if (g_win != NULL) {
			count = nano_skill_window_count(g_win);
			avg   = nano_skill_window_avg(g_win);
			nano_skill_window_reset(g_win);
		}
		if (g_win_mtx) nng_mtx_unlock(g_win_mtx);

		publish_metrics_line(ts_ms, count, avg);
	}
}

static void
on_batch_flush(void *const *items, const uint32_t *sizes, uint32_t count, void *ctx)
{
	(void) ctx;
	// Example sink: append each item as one JSONL line.
	for (uint32_t i = 0; i < count; i++) {
		(void) nano_file_append("/tmp/stream_plugin.jsonl", items[i], sizes[i]);
		(void) nano_file_append("/tmp/stream_plugin.jsonl", "\n", 1);
	}
	nano_log_info("flush count=%u", count);
}

static int
on_msg(const nano_msg *m)
{
	if (m == NULL) {
		return 0;
	}

	int rc;
	rc = nano_mqtt_publish("stream_forwarded", m->payload, m->payload_len, m->qos, m->retain);
	if (rc != 0) {
		nano_log_warn("Failed to publish message to stream_forwarded topic: rc=%d", rc);
		return rc;
	} else {
		nano_log_info("Published message to stream_forwarded topic");
	}

	if (g_win != NULL && m->payload != NULL && m->payload_len >= 1) {
		double v = (double) ((const uint8_t *) m->payload)[0];
		if (g_win_mtx) nng_mtx_lock(g_win_mtx);
		nano_skill_window_push(g_win, m->ts_ms, v);
		if (g_win_mtx) nng_mtx_unlock(g_win_mtx);
	}

	if (g_batch != NULL) {
		char rec[512];
		int  n = snprintf(rec, sizeof(rec),
		    "{\"ts\":%" PRIu64 ",\"topic\":\"%s\",\"len\":%u}",
		    m->ts_ms, m->topic ? m->topic : "", (unsigned) m->payload_len);
		if (n > 0 && (size_t)n < sizeof(rec)) {
			(void) nano_skill_batch_push(g_batch, rec, (uint32_t) n);
		} else {
			nano_log_warn("Plugin buffer truncated, dropped message on topic %s", m->topic);
		}
	}

	return 0;
}

static void
on_start(void)
{
	nano_log_info("plugin start");
	g_timer_stop = false;
	if (nng_mtx_alloc(&g_win_mtx) != 0) {
		g_win_mtx = NULL;
		nano_log_warn("failed to allocate metrics window mutex");
	}
	g_win   = nano_skill_window_tumbling_ms(METRICS_WINDOW_MS);
	g_batch = nano_skill_batch_open(200, 0, 3000, on_batch_flush, NULL);
	if (nng_thread_create(&g_timer_thr, metrics_timer, NULL) != 0) {
		g_timer_thr = NULL;
		nano_log_warn("failed to start metrics timer thread");
	}
}

static void
on_stop(void)
{
	nano_log_info("plugin stop");
	g_timer_stop = true;
	if (g_timer_thr != NULL) {
		nng_thread_destroy(g_timer_thr);
		g_timer_thr = NULL;
	}
	if (g_batch != NULL) {
		nano_skill_batch_close(g_batch);
		g_batch = NULL;
	}
	if (g_win != NULL) {
		nano_skill_window_free(g_win);
		g_win = NULL;
	}
	if (g_win_mtx != NULL) {
		nng_mtx_free(g_win_mtx);
		g_win_mtx = NULL;
	}
}

int
nano_plugin_init(void)
{
	int rv = nano_register_msg(on_msg);
	if (rv != 0) return rv;
	(void) nano_register_start(on_start);
	(void) nano_register_stop(on_stop);
	return 0;
}

