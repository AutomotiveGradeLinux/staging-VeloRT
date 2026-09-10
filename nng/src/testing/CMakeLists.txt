#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
nng_directory(testing`)

target_sources(nng_testing PRIVATE
        certs.c
        marry.c
        streams.c
        util.c)

target_include_directories(nng_testing PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
