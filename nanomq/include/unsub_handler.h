//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.
#ifndef MQTT_UNSUBSCRIBE_HANDLE_H
#define MQTT_UNSUBSCRIBE_HANDLE_H

#include <nng/nng.h>

#include "broker.h"
#include <nng/mqtt/packet.h>

int  decode_unsub_msg(nano_work *);
int  encode_unsuback_msg(nng_msg *, nano_work *);
int  unsub_ctx_handle(nano_work *);
void unsub_pkt_free(packet_unsubscribe *);

#endif // MQTT_UNSUBSCRIBE_HANDLE_H
