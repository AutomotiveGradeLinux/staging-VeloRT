//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "core/nng_impl.h"

#ifdef NNG_PLATFORM_WINDOWS

#ifndef _CRT_RAND_S
#define _CRT_RAND_S
#endif

#include <stdlib.h>

uint32_t
nni_random(void)
{
	unsigned val;

	// rand_s is claimed by Microsoft to generate cryptographically
	// secure numbers.  It also is claimed that this will only fail
	// for EINVAL if val is NULL (not the case here).  Other error
	// conditions might be possible, but we have no way to tell.
	// For now we just ignore that possibility.
	rand_s(&val);
	return ((uint32_t)val);
}

#endif // NNG_PLATFORM_WINDOWS