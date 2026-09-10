#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
nng_directory(ws)

if (NNG_TRANSPORT_WS OR NNG_TRANSPORT_WSS)
    set(WS_ON ON)
endif()

nng_defines_if(NNG_TRANSPORT_WS NNG_TRANSPORT_WS)
nng_defines_if(NNG_TRANSPORT_WSS NNG_TRANSPORT_WSS)
nng_sources_if(WS_ON websocket.c)
nng_headers_if(WS_ON nng/transport/ws/websocket.h)

nng_test_if(WS_ON ws_test)


