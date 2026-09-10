//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.
#include <stdlib.h>

#include "core/nng_impl.h"

#ifdef NNG_HAVE_ARC4RANDOM

uint32_t
nni_random(void)
{
	return (arc4random());
}

#endif