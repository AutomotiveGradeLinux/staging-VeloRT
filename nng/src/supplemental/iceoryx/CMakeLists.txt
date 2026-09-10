#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

if(NNG_ENABLE_ICEORYX)
    find_package(iceoryx_binding_c REQUIRED)
    find_package(iceoryx_platform REQUIRED)
    find_package(iceoryx_posh CONFIG REQUIRED)
    find_package(iceoryx_hoofs CONFIG REQUIRED)

    nng_sources(iceoryx_api.c)
    nng_sources(iceoryx_api.h)

    nng_test(iceoryx_api_test)
    nng_link_libraries(iceoryx_binding_c::iceoryx_binding_c)
endif()
