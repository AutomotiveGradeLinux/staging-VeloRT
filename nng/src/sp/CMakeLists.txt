#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

nng_directory(sp)

add_subdirectory(protocol)
add_subdirectory(transport)

nng_sources(
        protocol.c
        transport.c
        transport.h
)

nng_test(reconnect_stress_test)
