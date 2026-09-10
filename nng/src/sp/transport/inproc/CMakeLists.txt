#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
nng_directory(inproc)

nng_sources_if(NNG_TRANSPORT_INPROC inproc.c)
nng_headers_if(NNG_TRANSPORT_INPROC nng/transport/inproc/inproc.h)
nng_defines_if(NNG_TRANSPORT_INPROC NNG_TRANSPORT_INPROC)