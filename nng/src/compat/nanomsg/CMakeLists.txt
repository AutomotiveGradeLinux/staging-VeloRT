#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

nng_sources(nn.c)

set(NNG_TEST_PREFIX ${NNG_TEST_PREFIX}.compat.nanomsg)

nng_test(compat_msg_test)
nng_test(compat_tcp_test)
