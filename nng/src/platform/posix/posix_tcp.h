//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef PLATFORM_POSIX_TCP_H
#define PLATFORM_POSIX_TCP_H

#include "core/nng_impl.h"

#include "platform/posix/posix_aio.h"

struct nni_tcp_conn {
	nng_stream      stream;
	nni_posix_pfd * pfd;
	nni_list        readq;
	nni_list        writeq;
	bool            closed;
	nni_mtx         mtx;
	nni_aio *       dial_aio;
	nni_tcp_dialer *dialer;
	nni_reap_node   reap;
};

struct nni_tcp_dialer {
	nni_list                connq; // pending connections
	bool                    closed;
	bool                    nodelay;
	bool                    keepalive;
	struct sockaddr_storage src;
	size_t                  srclen;
	nni_mtx                 mtx;
	nni_atomic_u64          ref;
	nni_atomic_bool         fini;

	bool quickack;
	int  keepidle;
	int  keepintvl;
	int  keepcnt;
	int  sendtimeo;
	int  recvtimeo;

	char *bind_interface;
};

extern int  nni_posix_tcp_alloc(nni_tcp_conn **, nni_tcp_dialer *);
extern void nni_posix_tcp_init(nni_tcp_conn *, nni_posix_pfd *);
extern void nni_posix_tcp_start(nni_tcp_conn *, int, int);
extern void nni_posix_tcp_dialer_rele(nni_tcp_dialer *);

#endif // PLATFORM_POSIX_TCP_H