#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
nng_directory(socket)

nng_sources_if(NNG_TRANSPORT_FDC sockfd.c)
nng_defines_if(NNG_TRANSPORT_FDC NNG_TRANSPORT_FDC)
nng_test(sockfd_test)