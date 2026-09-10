//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.
#ifndef NNG_BRIDGE_H
#define NNG_BRIDGE_H

#include <stdio.h>
#include <stdlib.h>
#include "nng/mqtt/mqtt_client.h"
#include "nng/nng.h"
#include "nng/supplemental/nanolib/conf.h"
#include "nng/supplemental/util/platform.h"
#include "broker.h"
#include "pub_handler.h"

extern void nng_pub_handler(nano_work *);
extern int  nng_proxy_pub_init(conf_nng_pub_node *);
extern int  nng_proxy_sub_init(conf_nng_sub_node *, nano_work *);

#endif
