//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <time.h>

#ifdef NNG_PLATFORM_POSIX
#if !defined(NNG_USE_DEVURANDOM) && !defined(NNG_USE_GETENTROPY) && \
    !defined(NNG_USE_GETRANDOM) && !defined(NNG_USE_ARC4RANDOM)
#if defined(NNG_HAVE_GETENTROPY)
#define NNG_USE_GETENTROPY 1
#elif defined(NNG_HAVE_GETRANDOM)
#define NNG_USE_GETRANDOM 1
#elif defined(NNG_HAVE_ARC4RANDOM)
#define NNG_USE_ARC4RANDOM
#else
// It should never hurt to use DEVURANDOM, since if the device does not
// exist then we won't open it.  (Provided: it would be bad if the device
// exists but has somehow very very different semantics.  We don't know
// of any such concerns.)  This won't be used if any of the other options
// are defined and work.
#define NNG_USE_DEVURANDOM 1
#endif
#endif

#ifndef NNG_USE_CLOCKID
#if defined(__APPLE__)
#define NNG_USE_CLOCKID CLOCK_REALTIME
#elif defined(CLOCK_MONOTONIC)
#define NNG_USE_CLOCKID CLOCK_MONOTONIC
#elif defined(CLOCK_REALTIME)
#define NNG_USE_CLOCKID CLOCK_REALTIME
#else
#define NNG_USE_GETTIMEOFDAY
#endif
#endif

#define NNG_USE_POSIX_RESOLV_GAI 1

#endif // NNG_PLATFORM_POSIX
