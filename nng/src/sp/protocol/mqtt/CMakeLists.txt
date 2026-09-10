#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
nng_directory(mqtt)

if (NNG_PROTO_MQTT_BROKER)
    nng_sources_if(NNG_PROTO_MQTT_BROKER mqtt_parser.c nmq_mqtt.c auth_http.c)
    nng_headers_if(NNG_PROTO_MQTT_BROKER nng/protocol/mqtt/mqtt_parser.h nng/protocol/mqtt/nmq_mqtt.h)
    nng_defines_if(NNG_PROTO_MQTT_BROKER NNG_HAVE_MQTT_BROKER)
endif ()

nng_test(mqtt_parser_test)
nng_test(auth_http_test)
