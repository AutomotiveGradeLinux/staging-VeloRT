#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
nng_directory(pair0)

nng_sources_if(NNG_PROTO_PAIR0 pair.c)
nng_headers_if(NNG_PROTO_PAIR0 nng/protocol/pair0/pair.h)
nng_defines_if(NNG_PROTO_PAIR0 NNG_HAVE_PAIR0)
nng_test(pair0_test)
