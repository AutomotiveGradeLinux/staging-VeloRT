#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
nng_directory(mqtt)

nng_sources_if(
    NNG_PROTO_MQTT_CLIENT
    mqtt_client.c
    sqlite_handler.c
    sqlite_handler.h
)
nng_headers_if(NNG_PROTO_MQTT_CLIENT nng/mqtt/mqtt_client.h)
nng_defines_if(NNG_PROTO_MQTT_CLIENT NNG_HAVE_MQTT_CLIENT)
message(" Check MQTT_QUIC_CLIENT support: ${NNG_PROTO_MQTT_QUIC_CLIENT} ")
if (NNG_PROTO_MQTT_QUIC_CLIENT)
    nng_sources_if(NNG_PROTO_MQTT_QUIC_CLIENT mqtt_quic_client.c)
    nng_headers_if(NNG_PROTO_MQTT_QUIC_CLIENT nng/mqtt/mqtt_quic_client.h)
    nng_defines_if(NNG_PROTO_MQTT_QUIC_CLIENT NNG_HAVE_MQTT_QUIC_CLIENT)
endif()
