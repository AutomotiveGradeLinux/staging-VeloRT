#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
nng_directory(mqtts)

nng_sources_if(NNG_TRANSPORT_MQTT_TLS mqtt_tls.c)
nng_headers_if(NNG_TRANSPORT_MQTT_TLS nng/mqtt/transport/tls/mqtt_tls.h)
nng_defines_if(NNG_TRANSPORT_MQTT_TLS NNG_TRANSPORT_MQTT_TLS)
