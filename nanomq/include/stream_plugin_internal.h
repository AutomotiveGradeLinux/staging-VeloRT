//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef NANOMQ_STREAM_PLUGIN_INTERNAL_H
#define NANOMQ_STREAM_PLUGIN_INTERNAL_H

#include "nng/supplemental/nanolib/conf.h"
#include "broker.h"

#ifdef __cplusplus
extern "C" {
#endif

int  stream_plugin_load_all(conf *cfg);
int  stream_plugin_start_all(conf *cfg);
void stream_plugin_stop_all(conf *cfg);
void stream_plugin_unload_all(conf *cfg);

// Dispatch from broker SEND path (before hook_entry).
void stream_plugin_pub_dispatch_from_work(nano_work *work);

#ifdef __cplusplus
}
#endif

#endif

