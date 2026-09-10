#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

if (NNG_SUPP_WEBSOCKET)
    nng_sources(websocket.c websocket.h)
else ()
    nng_sources(stub.c)
endif ()
nng_test(wssfile_test)
nng_test(websocket_test)
