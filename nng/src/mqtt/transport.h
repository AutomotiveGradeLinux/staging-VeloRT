//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef PROTOCOL_MQTT_TRANSPORT_H
#define PROTOCOL_MQTT_TRANSPORT_H

#include "core/options.h"

// These APIs are used by the framework internally, and not for use by
// transport implementations.
extern nni_sp_tran *nni_mqtt_tran_find(nni_url *);
extern void         nni_mqtt_tran_sys_init(void);
extern void         nni_mqtt_tran_sys_fini(void);
extern void         nni_mqtt_tran_register(nni_sp_tran *);

#endif // PROTOCOL_MQTT_TRANSPORT_H
