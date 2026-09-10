#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
nng_directory(mqtt)

nng_sources_if(NNG_TRANSPORT_MQTT_TCP mqtt_tcp.c)
nng_headers_if(NNG_TRANSPORT_MQTT_TCP nng/mqtt/transport/tcp/mqtt_tcp.h)
nng_defines_if(NNG_TRANSPORT_MQTT_TCP NNG_TRANSPORT_MQTT_TCP)
nng_test_if(NNG_TRANSPORT_MQTT_TCP mqtt_tcp_test)
