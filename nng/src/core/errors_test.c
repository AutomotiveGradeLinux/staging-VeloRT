//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <errno.h>
#include <string.h>

#include <nuts.h>

static void
test_known_errors(void)
{
	NUTS_MATCH(nng_strerror(NNG_ECLOSED), "Object closed");
	NUTS_MATCH(nng_strerror(NNG_ETIMEDOUT), "Timed out");
}

static void
test_unknown_errors(void)
{
	for (unsigned i = 1; i < 0x1000000; i = i * 2 + 100) {
		NUTS_TRUE(nng_strerror(i) != NULL);
	}
}

static void
test_system_errors(void)
{
	NUTS_MATCH(nng_strerror(NNG_ESYSERR + ENOENT), strerror(ENOENT));
	NUTS_MATCH(nng_strerror(NNG_ESYSERR + EINVAL), strerror(EINVAL));
}

NUTS_TESTS = {
	{ "known errors", test_known_errors },
	{ "unknown errors", test_unknown_errors },
	{ "system errors", test_system_errors },
	{ NULL, NULL },
};
