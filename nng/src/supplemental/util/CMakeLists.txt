#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

nng_sources(idhash.c options.c platform.c)
nng_headers(
	nng/supplemental/util/idhash.h
	nng/supplemental/util/options.h
	nng/supplemental/util/platform.h)
nng_test(idhash_test)

# Test for nng_id_count function
if (NNG_TESTS)
    nng_test(idhash_count_test)
endif()
