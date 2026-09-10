//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.
#ifndef AWS_BRIDGE_H
#define AWS_BRIDGE_H

#include "nng/supplemental/nanolib/conf.h"
#include "broker.h"

extern int  aws_bridge_client(conf_bridge_node *node);
extern void aws_bridge_forward(nano_work *work);

#endif
