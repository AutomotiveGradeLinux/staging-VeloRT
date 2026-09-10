//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.
#include <nng/nng.h>
#include <nuts.h>

void
test_ipc_win_sec(void)
{
	char                 address[64];
	nng_stream_listener *l;
	int                  x;

	nuts_scratch_addr("ipc", sizeof(address), address);
	NUTS_PASS(nng_stream_listener_alloc(&l, address));
	NUTS_FAIL(nng_stream_listener_set_ptr(
	              l, NNG_OPT_IPC_SECURITY_DESCRIPTOR, &x),
	    NNG_ENOTSUP);
	nng_stream_listener_free(l);
}

NUTS_TESTS = {
	{ "ipc security descriptor", test_ipc_win_sec },
	{ NULL, NULL },
};
