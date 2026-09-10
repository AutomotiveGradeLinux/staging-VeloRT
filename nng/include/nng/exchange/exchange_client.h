//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef EXCHANGE_CLIENT_H
#define EXCHANGE_CLIENT_H

#ifdef __cplusplus
extern "C" {
#endif

#define nng_exchange_self                0
#define nng_exchange_self_name           "exchange-client"
#define nng_exchange_peer                0
#define nng_exchange_peer_name           "exchange-server"
#define nng_opt_exchange_add             "exchange-client-add"

#define NNG_EXCHANGE_SELF                0
#define NNG_EXCHANGE_SELF_NAME           "exchange-client"
#define NNG_EXCHANGE_PEER                0
#define NNG_EXCHANGE_PEER_NAME           "exchange-server"
#define NNG_OPT_EXCHANGE_BIND            "exchange-client-bind"
#define NNG_OPT_EXCHANGE_GET_EX_QUEUE    "exchange-client-get-ex-queue"
#define NNG_OPT_EXCHANGE_GET_RBMSGMAP    "exchange-client-get-rbmsgmap"
#define NNG_OPT_EXCHANGE_START_LIMIT_TIMER "exchange-client-start-limit-timer"

NNG_DECL int nng_exchange_client_open(nng_socket *sock);

#ifndef nng_exchange_open
#define nng_exchange_open nng_exchange_client_open
#endif

#ifdef __cplusplus
}
#endif

#endif // #define EXCHANGE_CLIENT_H

