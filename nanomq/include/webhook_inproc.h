//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.
#ifndef WEBHOOK_INPROC_H
#define WEBHOOK_INPROC_H

#include "nng/supplemental/nanolib/conf.h"
#include "nng/nng.h"

#define HOOK_IPC_URL "ipc:///tmp/nanomq_hook.ipc"
#define EXCHANGE_IPC_URL "ipc:///tmp/nanomq_exchange.ipc"
#define EXTERNAL2NANO_IPC "EX2NANO"

extern int start_hook_service(conf *conf);
extern int stop_hook_service(void);

#endif
