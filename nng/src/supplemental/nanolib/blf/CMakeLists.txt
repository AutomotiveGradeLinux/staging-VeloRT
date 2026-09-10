#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

if(NNG_ENABLE_BLF)
    nng_sources(blf.cc)
    find_package(Vector_BLF REQUIRED)
    nng_include_directories(${Vector_BLF_INCLUDE_DIRS})
    nng_link_libraries(${Vector_BLF_LIBRARIES})
endif()
