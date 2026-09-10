#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
nng_directory(mqtts)

if (NNG_TRANSPORT_MQTT_BROKER_TLS)
    nng_sources_if(NNG_TRANSPORT_MQTT_BROKER_TLS broker_tls.c)
    nng_headers_if(NNG_TRANSPORT_MQTT_BROKER_TLS nng/transport/mqtts/broker_tls.h)
    nng_defines_if(NNG_TRANSPORT_MQTT_BROKER_TLS NNG_TRANSPORT_MQTT_BROKER_TLS)
endif()