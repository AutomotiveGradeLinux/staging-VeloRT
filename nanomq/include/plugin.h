//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.
#ifndef NANOMQ_PLUGIN_H
#define NANOMQ_PLUGIN_H

#include "nng/nng.h"
#include "nng/supplemental/nanolib/conf.h"
#include "nng/supplemental/nanolib/log.h"

struct nano_plugin {
	char *path;
	int (*init)(void);
};

enum hook_point {
	HOOK_USER_PROPERTY,
};

struct plugin_hook {
	unsigned int point;
	int (*cb)(void *data);
};

extern int plugin_hook_register(unsigned int point, int (*cb)(void *data));
extern int plugin_hook_call(unsigned int point, void *data);
extern int plugin_register(char *path);
extern void plugins_clear();
extern int plugin_init(struct nano_plugin *plugin);

#endif // NANOMQ_PLUGIN_H
