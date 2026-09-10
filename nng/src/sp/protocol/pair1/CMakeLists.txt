#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
nng_directory(pair1)

# XXX: break pair1_poly into an ifdef.
nng_sources_if(NNG_PROTO_PAIR1 pair.c pair1_poly.c)
nng_headers_if(NNG_PROTO_PAIR1 nng/protocol/pair1/pair.h)
nng_defines_if(NNG_PROTO_PAIR1 NNG_HAVE_PAIR1)

nng_test(pair1_test)
nng_test(pair1_poly_test)