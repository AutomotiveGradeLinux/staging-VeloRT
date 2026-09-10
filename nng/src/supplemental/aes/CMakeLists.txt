#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

if (NNG_ENABLE_PARQUET OR ENABLE_LICENSE_STD OR ENABLE_LICENSE_DK)
    nng_sources(aes.c aes.h)
    nng_test(aes_test)
endif ()
