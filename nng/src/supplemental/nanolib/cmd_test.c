//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.
#include "nng/supplemental/nanolib/cmd.h"

#include "nuts.h"

void
test_nano_cmd(void)
{
#ifndef NNG_PLATFORM_WINDOWS
	char *cmd_null = NULL;
	NUTS_FAIL(nano_cmd_run(cmd_null), -1);

	char *cmd_invalid = "just test";
	NUTS_FAIL(nano_cmd_frun(cmd_invalid), -1);

	char *cmd = "whoami";
	NUTS_PASS(nano_cmd_frun(cmd));

	nano_cmd_cleanup();
#endif
}

NUTS_TESTS = {
   {"nano cmd", test_nano_cmd},
   {NULL, NULL} 
};