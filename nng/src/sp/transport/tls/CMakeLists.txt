#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
nng_directory(tls)

nng_sources_if(NNG_TRANSPORT_TLS tls.c)
nng_headers_if(NNG_TRANSPORT_TLS nng/transport/tls/tls.h)
nng_defines_if(NNG_TRANSPORT_TLS NNG_TRANSPORT_TLS)
nng_test_if(NNG_ENABLE_TLS tls_tran_test)
