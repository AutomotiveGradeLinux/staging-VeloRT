#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

option (NNG_ENABLE_HTTP "Enable HTTP API" ON)
if (NNG_ENABLE_HTTP)
    set(NNG_SUPP_HTTP ON)
endif()
mark_as_advanced(NNG_ENABLE_HTTP)

nng_sources(http_public.c http_api.h)
nng_headers(nng/supplemental/http/http.h)

nng_defines_if(NNG_SUPP_HTTP NNG_SUPP_HTTP)
nng_sources_if(NNG_SUPP_HTTP
        http_client.c
        http_chunk.c
        http_conn.c
        http_msg.c
        http_public.c
        http_schemes.c
        http_server.c)

nng_test_if(NNG_SUPP_HTTP http_conn_test)
