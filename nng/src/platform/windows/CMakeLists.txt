#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
if (NNG_PLATFORM_WINDOWS)
    nng_check_sym(InitializeConditionVariable windows.h NNG_HAVE_CONDVAR)
    nng_check_sym(timespec_get time.h NNG_HAVE_TIMESPEC_GET)
    nng_check_sym(snprintf stdio.h NNG_HAVE_SNPRINTF)
    if (NOT NNG_HAVE_CONDVAR OR NOT NNG_HAVE_SNPRINTF OR NOT NNG_HAVE_TIMESPEC_GET)
        message(FATAL_ERROR
                "Modern Windows API support is missing. "
                "Versions of Windows prior to Vista are not supported.  "
                "Further, the legacy MinGW environments are not supported. "
                "Ensure you have at least Windows Vista or newer, and are "
                "using either Visual Studio 2013 or compatible compiler, "
                "and are also using the universal C runtime (UCRT).")
    endif ()
    nng_link_libraries(ws2_32 mswsock advapi32)

    nng_sources(
            win_impl.h
            win_ipc.h
            win_tcp.h
            win_clock.c
            win_debug.c
            win_file.c
            win_io.c
            win_ipcconn.c
            win_ipcdial.c
            win_ipclisten.c
            win_pipe.c
            win_rand.c
            win_resolv.c
            win_sockaddr.c
            win_socketpair.c
            win_tcp.c
            win_tcpconn.c
            win_tcpdial.c
            win_tcplisten.c
            win_thread.c
            win_udp.c
    )

    nng_test(win_ipc_sec_test)

endif ()
