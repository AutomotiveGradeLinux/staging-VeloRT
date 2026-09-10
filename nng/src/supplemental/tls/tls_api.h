//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef NNG_SUPPLEMENTAL_TLS_TLS_API_H
#define NNG_SUPPLEMENTAL_TLS_TLS_API_H

#include <nng/supplemental/tls/tls.h>

// The implementation supplies this function to create the TLS connection
// object.  All fields will be zeroed.
extern int nni_tls_dialer_alloc(nng_stream_dialer **, const nng_url *);
extern int nni_tls_listener_alloc(nng_stream_listener **, const nng_url *);

#endif // NNG_SUPPLEMENTAL_TLS_TLS_API_H
