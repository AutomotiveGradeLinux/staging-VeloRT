//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "core/nng_impl.h"

#ifdef NNG_PLATFORM_POSIX

#include <stdlib.h>

// POSIX memory allocation.  This is pretty much standard C.
void *
nni_alloc(size_t sz)
{
	void *ret;

	ret = (sz > 0 ? malloc(sz) : NULL);
	if (ret == NULL) {
		log_error("malloc failed sz: %lu\n", sz);
	}

	return ret;
}

void *
nni_zalloc(size_t sz)
{
	void *ret;
	ret = (sz > 0 ? calloc(1, sz) : NULL);
	if (ret == NULL) {
		log_error("calloc failed sz: %lu\n", sz);
	}

	return ret;
}

void
nni_free(void *ptr, size_t size)
{
	NNI_ARG_UNUSED(size);
	free(ptr);
}

#endif // NNG_PLATFORM_POSIX
