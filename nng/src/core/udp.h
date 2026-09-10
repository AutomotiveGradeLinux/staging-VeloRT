//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef CORE_UDP_H
#define CORE_UDP_H

#include "core/nng_impl.h"

// These are interfaces we use for UDP internally.  These are not exposed
// to the public API.

typedef struct nni_udp_conn nni_udp_conn;
struct nni_udp_conn {
	nng_stream    stream;
	nni_plat_udp *u;
	void *        d; // udp_dialer
	nni_reap_node reap;
};

extern int nni_udp_dialer_alloc(nng_stream_dialer **, const nng_url *);
// TODO extern int nni_udp_listener_alloc(nng_stream_listener **, const nng_url *);

#endif // CORE_UDP_H

