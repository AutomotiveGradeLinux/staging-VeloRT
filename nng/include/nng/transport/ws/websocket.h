//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef NNG_TRANSPORT_WS_WEBSOCKET_H
#define NNG_TRANSPORT_WS_WEBSOCKET_H

#include <nng/nng.h>

#ifdef __cplusplus
extern "C" {
#endif

// WebSocket transport.  This is used for communication via WebSocket.

// These aliases are for WSS naming consistency.
#define NNG_OPT_WSS_REQUEST_HEADERS NNG_OPT_WS_REQUEST_HEADERS
#define NNG_OPT_WSS_RESPONSE_HEADERS NNG_OPT_WS_RESPONSE_HEADERS

#ifndef NNG_ELIDE_DEPRECATED
NNG_DECL int nng_ws_register(void);
NNG_DECL int nng_wss_register(void);
#endif

#ifdef __cplusplus
}
#endif

#endif // NNG_TRANSPORT_WS_WEBSOCKET_H
