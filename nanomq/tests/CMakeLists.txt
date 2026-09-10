#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

nanomq_test(sub_handler_test)
nanomq_test(unsub_handler_test)
nanomq_test(pub_handler_test)
nanomq_test(broker_test)
nanomq_test(webhook_test)
nanomq_test(webhook_base62_test)
nanomq_test(webhook_base64_test)
nanomq_test(http_server_test)
nanomq_test(bridge_test)
nanomq_test(rule_engine_test)
nanomq_test(hashmap_test)
nanomq_test(broker_tls_test)
nanomq_test(broker_auth_anon_test)
nanomq_test(bridge_tls_test)
nanomq_test(bridge_rap_rh_test)
nanomq_test(mqtt_api_test)
nanomq_test(nmq_ws_test)
nanomq_test(properties_test)
if(ENABLE_PARQUET)
    nanomq_test(parquet_test)
endif()
if(NNG_ENABLE_QUIC)
    nanomq_test(quic_smoke_test)
endif()

