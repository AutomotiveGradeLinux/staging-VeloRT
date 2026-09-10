#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
nng_directory(platform)

add_subdirectory(posix)
add_subdirectory(windows)

nng_test(platform_test)
nng_test(resolver_test)
nng_test(udp_test)
nng_test(tcp_stream_test)
nng_test(ipc_stream_test)
