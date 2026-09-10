#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
nng_directory(pubsub0)

nng_sources_if(NNG_PROTO_PUB0 pub.c)
nng_headers_if(NNG_PROTO_PUB0 nng/protocol/pubsub0/pub.h)
nng_defines_if(NNG_PROTO_PUB0 NNG_HAVE_PUB0)

nng_sources_if(NNG_PROTO_SUB0 sub.c xsub.c)
nng_headers_if(NNG_PROTO_SUB0 nng/protocol/pubsub0/sub.h)
nng_defines_if(NNG_PROTO_SUB0 NNG_HAVE_SUB0)

nng_test(pub_test)
nng_test(sub_test)
nng_test(xsub_test)
