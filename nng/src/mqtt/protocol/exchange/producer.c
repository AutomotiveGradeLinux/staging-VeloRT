//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.
#include "nng/exchange/producer.h"
#include "core/nng_impl.h"

int
producer_init(producer_t **pp,
			  int (*match)(void *data),
			  int (*target)(void *data))
{
	producer_t *newP = NULL;

	if (match == NULL || target == NULL || pp == NULL) {
		return -1;
	}

	newP = (producer_t *)nng_alloc(sizeof(producer_t));
	if (newP == NULL) {
		log_error("producer init failed! no memory!\n");
		return -1;
	}

	newP->match = match;
	newP->target = target;

	*pp = newP;

	return 0;
}

int
producer_release(producer_t *p)
{
	if (p == NULL) {
		return -1;
	}

	nng_free(p, sizeof(producer_t));

	return 0;
}
