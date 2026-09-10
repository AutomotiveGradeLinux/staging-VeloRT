//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef PLATFORM_POSIX_PEERID_H
#define PLATFORM_POSIX_PEERID_H

// This file defines structures we will use for emulating asynchronous I/O
// on POSIX.  POSIX lacks the support for callback based asynchronous I/O
// that we have on Windows, although it has a non-widely support aio layer
// that is not very performant on many systems.   So we emulate this using
// one of several possible different backends.

#include "core/nng_impl.h"
#include <sys/types.h>

int nni_posix_peerid(
    int fd, uint64_t *euid, uint64_t *egid, uint64_t *prid, uint64_t *znid);

#endif // PLATFORM_POSIX_PEERID_H