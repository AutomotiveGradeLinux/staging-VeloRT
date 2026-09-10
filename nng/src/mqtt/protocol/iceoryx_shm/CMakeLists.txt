#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.


if (NNG_ENABLE_ICEORYX)
    nng_headers(nng/iceoryx_shm/iceoryx_shm.h)
    nng_sources(iceoryx_shm.c)

    #nng_test(iceoryx_shm_test)
endif ()

