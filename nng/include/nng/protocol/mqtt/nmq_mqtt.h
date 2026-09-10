//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef NNG_PROTOCOL_MQTT_BROKER_H
#define NNG_PROTOCOL_MQTT_BROKER_H

#ifdef __cplusplus
extern "C" {
#endif

NNG_DECL int nng_nmq_tcp0_open(nng_socket *);

#ifndef nng_nmq_tcp_open
#define nng_nmq_tcp_open nng_nmq_tcp0_open
#endif

#define NNG_NMQ_TCP_SELF 0x31
#define NNG_NMQ_TCP_PEER 0x30
#define NNG_NMQ_TCP_SELF_NAME "nmq_broker"
#define NNG_NMQ_TCP_PEER_NAME "nmq_client"


#define NMQ_OPT_MQTT_PIPES "mqtt-clients-pipes"
#define NMQ_OPT_MQTT_QOS_DB "mqtt-clients-qos-db"

#ifdef __cplusplus
}
#endif

#endif // NNG_PROTOCOL_MQTT_BROKER_H
