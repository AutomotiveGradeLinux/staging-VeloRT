//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef NNG_TRANSPORT_MQTT_BROKER_TLS_TCP_H
#define NNG_TRANSPORT_MQTT_BROKER_TLS_TCP_H

#include <nng/nng.h>

#ifdef __cplusplus
extern "C" {
#endif

// TLS transport.  This is used for communication over TLS.

#ifndef NNG_ELIDE_DEPRECATED
NNG_DECL int nmq_mqtt_tls_register(void);
#endif

#ifdef __cplusplus
}
#endif

#endif // NNG_TRANSPORT_MQTT_BROKER_TLS_TCP_H
