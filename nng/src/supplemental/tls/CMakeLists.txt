#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

if (NNG_ENABLE_TLS)
    set(NNG_TLS_ENGINES mbed none)
    # We assume Mbed for now.  (Someday replaced perhaps with Bear.)
    set(NNG_TLS_ENGINE mbed CACHE STRING "TLS engine to use.")
    set_property(CACHE NNG_TLS_ENGINE PROPERTY STRINGS ${NNG_TLS_ENGINES})
else ()
    set(NNG_TLS_ENGINE none)
endif ()

# default TLS implementation for now is Mbed.
nng_headers(nng/supplemental/tls/tls.h)
nng_headers(nng/supplemental/tls/engine.h)

if (NOT NNG_TLS_ENGINE STREQUAL "none")
    nng_test(tls_test)
endif ()

add_subdirectory(mbedtls)
add_subdirectory(openssl)

nng_sources(tls_common.c)
nng_sources(tls_api.h)
