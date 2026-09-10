//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "http_api.h"

#include <nuts.h>

extern void nni_http_conn_test_set_fe_timeout(nng_duration);
extern void nni_http_conn_test_reset_fini_count(void);
extern int  nni_http_conn_test_get_fini_count(void);
extern void nni_http_conn_test_stop_write(nni_http_conn *);

void
test_http_conn_init_schedule_failure(void)
{
	nni_http_conn *conn = NULL;

	NUTS_PASS(nni_init());
	nni_http_conn_test_set_fe_timeout(NNG_DURATION_ZERO);
	NUTS_FAIL(nni_http_conn_init(&conn, NULL), NNG_ETIMEDOUT);
	nni_http_conn_test_set_fe_timeout(NNG_DURATION_INFINITE);
	NUTS_TRUE(conn == NULL);
}

void
test_http_conn_fini_stopped_write(void)
{
	nni_http_conn *conn = NULL;

	NUTS_PASS(nni_init());
	nni_http_conn_test_reset_fini_count();
	NUTS_PASS(nni_http_conn_init(&conn, NULL));
	nni_http_conn_test_stop_write(conn);
	nni_http_conn_fini(conn);
	nni_reap_drain();
	NUTS_TRUE(nni_http_conn_test_get_fini_count() == 1);
}

NUTS_TESTS = {
	{ "HTTP connection init schedule failure",
	    test_http_conn_init_schedule_failure },
	{ "HTTP connection fini with stopped write AIO",
	    test_http_conn_fini_stopped_write },
	{ NULL, NULL },
};
