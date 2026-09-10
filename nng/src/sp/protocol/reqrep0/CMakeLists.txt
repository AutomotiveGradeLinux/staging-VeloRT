#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
nng_directory(reqrep0)

nng_sources_if(NNG_PROTO_REQ0 req.c xreq.c)
nng_headers_if(NNG_PROTO_REQ0 nng/protocol/reqrep0/req.h)
nng_defines_if(NNG_PROTO_REQ0 NNG_HAVE_REQ0)

nng_sources_if(NNG_PROTO_REP0 rep.c xrep.c)
nng_headers_if(NNG_PROTO_REP0 nng/protocol/reqrep0/rep.h)
nng_defines_if(NNG_PROTO_REP0 NNG_HAVE_REP0)

nng_test(req_test)
nng_test(rep_test)
nng_test(xrep_test)
nng_test(xreq_test)
