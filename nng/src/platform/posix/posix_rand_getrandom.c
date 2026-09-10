//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <stddef.h>
#include <sys/random.h>

#include "core/nng_impl.h"

#ifdef NNG_HAVE_GETRANDOM

uint32_t
nni_random(void)
{
	uint32_t val;

	// Documentation claims that as long as we are not using
	// GRND_RANDOM and buflen < 256, this should never fail.
	// The exception here is that we could fail if for some
	// reason we got a signal while blocked at very early boot
	// (i.e. /dev/urandom was not yet seeded).
	if (getrandom(&val, sizeof(val), 0) != sizeof(val)) {
		nni_panic("getrandom failed");
	}
	return (val);
}

#endif
