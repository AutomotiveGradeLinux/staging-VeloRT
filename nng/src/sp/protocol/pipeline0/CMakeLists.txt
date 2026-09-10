#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
nng_directory(pipeline0)

nng_sources_if(NNG_PROTO_PUSH0 push.c)
nng_headers_if(NNG_PROTO_PUSH0 nng/protocol/pipeline0/push.h)
nng_defines_if(NNG_PROTO_PUSH0 NNG_HAVE_PUSH0)

nng_sources_if(NNG_PROTO_PULL0 pull.c)
nng_headers_if(NNG_PROTO_PULL0 nng/protocol/pipeline0/pull.h)
nng_defines_if(NNG_PROTO_PULL0 NNG_HAVE_PULL0)

nng_test(pull_test)
nng_test(push_test)
