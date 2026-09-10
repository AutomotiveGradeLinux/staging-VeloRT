#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

message(" Check MQTT_QUIC_TRANSPORT support: ${NNG_TRANSPORT_MQTT_QUIC} ")

if (NNG_TRANSPORT_MQTT_QUIC)
    nng_sources_if(NNG_TRANSPORT_MQTT_QUIC mqtt_quic.c)
    nng_headers_if(NNG_TRANSPORT_MQTT_QUIC nng/mqtt/transport/quic/mqtt_quic.h)
    nng_defines_if(NNG_TRANSPORT_MQTT_QUIC NNG_TRANSPORT_MQTT_QUIC)
endif()
