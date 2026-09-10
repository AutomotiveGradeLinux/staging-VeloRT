#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

nng_directory(perf)

if (NNG_TESTS)
     macro (add_nng_perf NAME)
        add_executable (${NAME} perf.c)
        if(NNG_ENABLE_QUIC)
            target_link_libraries (${NAME} nng nng_private msquic OpenSSLQuic)
        else()
            target_link_libraries (${NAME} nng nng_private)
        endif()
        
    endmacro (add_nng_perf)

    add_nng_perf(remote_lat)
    add_nng_perf(local_lat)
    add_nng_perf(local_thr)
    add_nng_perf(remote_thr)
    add_nng_perf(inproc_thr)
    add_nng_perf(inproc_lat)

    add_test (NAME nng.inproc_lat COMMAND inproc_lat 64 10000)
    set_tests_properties (nng.inproc_lat PROPERTIES TIMEOUT 30)

    add_test (NAME nng.inproc_thr COMMAND inproc_thr 1400 10000)
    set_tests_properties (nng.inproc_thr PROPERTIES TIMEOUT 30)

    add_executable (pubdrop pubdrop.c)
    if(NNG_ENABLE_QUIC)
        target_link_libraries(pubdrop nng nng_private msquic OpenSSLQuic)
    else()
        target_link_libraries(pubdrop nng nng_private)
    endif()
    
endif ()
