//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef PLATFORM_WIN_WINTCP_H
#define PLATFORM_WIN_WINTCP_H

// This header file is private to the TCP support for Windows.

#include "core/nng_impl.h"

struct nni_tcp_conn {
	nng_stream        ops;
	SOCKET            s;
	nni_win_io        recv_io;
	nni_win_io        send_io;
	nni_win_io        conn_io;
	nni_list          recv_aios;
	nni_list          send_aios;
	nni_aio          *conn_aio;
	SOCKADDR_STORAGE  sockname;
	SOCKADDR_STORAGE  peername;
	nni_tcp_dialer   *dialer;
	nni_tcp_listener *listener;
	int               recv_rv;
	int               send_rv;
	int               conn_rv;
	bool              closed;
	char              buf[512]; // to hold acceptex results
	bool              sending;
	bool              recving;
	nni_mtx           mtx;
	nni_cv            cv;
};

extern int nni_win_tcp_init(nni_tcp_conn **, SOCKET);

#endif // NNG_PLATFORM_WIN_WINTCP_H
