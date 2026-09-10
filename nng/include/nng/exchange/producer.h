//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.
#ifndef PRODUCER_H
#define PRODUCER_H

#include<stddef.h>

typedef struct producer_s producer_t;
struct producer_s {
	/*
	 * return: 0: success, -1: failed
	 */
	int (*match)(void *data);
	/*
	 * return: 0: continue, -1: stop and return
	 */
	int (*target)(void *data);
};

int producer_init(producer_t **p,
				  int (*match)(void *data),
				  int (*target)(void *data));
int producer_release(producer_t *p);

#endif
