#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

nng_sources_if(NNG_SUPP_BASE64
        base64.c
        base64.h)

nng_test_if(NNG_SUPP_BASE64
        base64_test)
