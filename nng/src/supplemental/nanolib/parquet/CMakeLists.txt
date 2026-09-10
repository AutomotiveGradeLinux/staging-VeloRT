#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

if(NNG_ENABLE_PARQUET)
    nng_sources(parquet.cc parquet_file_queue.cc)
    find_package(Arrow CONFIG REQUIRED)
    find_package(Parquet CONFIG REQUIRED)
    if(ENABLE_PARQUET_SHARED)
        nng_link_libraries(arrow_shared parquet_shared)
    else(ENABLE_PARQUET_SHARED)
        nng_link_libraries(arrow_static parquet_static)
    endif(ENABLE_PARQUET_SHARED)
    # nng_test(parquet_test)
endif()
