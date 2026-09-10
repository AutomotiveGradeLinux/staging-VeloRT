//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef NNG_SUPPLEMENTAL_WEBSOCKET_WEBSOCKET_H
#define NNG_SUPPLEMENTAL_WEBSOCKET_WEBSOCKET_H

#include <stdbool.h>

typedef struct nni_ws          nni_ws;
typedef struct nni_ws_listener nni_ws_listener;
typedef struct nni_ws_dialer   nni_ws_dialer;

// Internal option, not for normal use (at present).  This sets the
// dialer/listener into message mode.  This is used by the SP transport.
// This is a boolean.
#define NNI_OPT_WS_MSGMODE "ws:msgmode"

// Specify URL as ws://[<host>][:port][/path]
// If host is missing, INADDR_ANY is assumed.  If port is missing,
// then either 80 or 443 are assumed.  Note that ws:// means listen
// on INADDR_ANY port 80, with path "/".  For connect side, INADDR_ANY
// makes no sense.  (TBD: return NNG_EADDRINVAL, or try loopback?)

// Much of the websocket API is still "private", meaning you should not
// rely upon it being around.
extern int nni_ws_listener_alloc(nng_stream_listener **, const nni_url *);
extern int nni_ws_dialer_alloc(nng_stream_dialer **, const nni_url *);

#endif // NNG_SUPPLEMENTAL_WEBSOCKET_WEBSOCKET_H
