//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.
#include "nng/supplemental/nanolib/env.h"

#include "nuts.h"

void
test_env(void)
{
	conf *conf_test = NULL;
	NUTS_TRUE((conf_test = nng_zalloc(sizeof(conf))) != NULL);
	conf_init(conf_test);
	read_env_conf(conf_test);
	conf_fini(conf_test);
}

NUTS_TESTS = {
   {"test env", test_env},
   {NULL, NULL} 
};