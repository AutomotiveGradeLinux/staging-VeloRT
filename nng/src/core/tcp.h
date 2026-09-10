//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef CORE_TCP_H
#define CORE_TCP_H

#include "core/nng_impl.h"

// These are interfaces we use for TCP internally.  These are not exposed
// to the public API.

extern int nni_tcp_dialer_alloc(nng_stream_dialer **, const nng_url *);
extern int nni_tcp_listener_alloc(nng_stream_listener **, const nng_url *);

#endif // CORE_TCP_H
