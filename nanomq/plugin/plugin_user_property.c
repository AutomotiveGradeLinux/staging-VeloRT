//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.
#include <stdio.h>
#include <stdlib.h>
#include "include/plugin.h"

/*
 * How to compile:
 * gcc -I../ -fPIC -shared plugin_user_property.c -o plugin_user_property.so
 * or
 * gcc -I.. -I../../nng/include -fPIC -shared plugin_user_property.c -o plugin_user_property.so
 */

int cb(void *data)
{
	char **property = data;
	if (property != NULL) {
		property[0] = malloc(strlen("alpha") + 1);
		strcpy(property[0], "alpha");
		property[1] = malloc(strlen("beta") + 1);
		strcpy(property[1], "beta");
	}

	return 0;
}

int nano_plugin_init()
{
	plugin_hook_register(HOOK_USER_PROPERTY, cb);
	return 0;
}
