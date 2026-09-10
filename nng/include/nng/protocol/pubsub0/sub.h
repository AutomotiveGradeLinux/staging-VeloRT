//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef NNG_PROTOCOL_PUBSUB0_SUB_H
#define NNG_PROTOCOL_PUBSUB0_SUB_H

#ifdef __cplusplus
extern "C" {
#endif

NNG_DECL int nng_sub0_open(nng_socket *);

NNG_DECL int nng_sub0_open_raw(nng_socket *);

NNG_DECL int nng_sub0_socket_subscribe(
    nng_socket id, const void *buf, size_t sz);
NNG_DECL int nng_sub0_socket_unsubscribe(
    nng_socket id, const void *buf, size_t sz);
NNG_DECL int nng_sub0_ctx_subscribe(nng_ctx id, const void *buf, size_t sz);
NNG_DECL int nng_sub0_ctx_unsubscribe(nng_ctx id, const void *buf, size_t sz);

#ifndef nng_sub_open
#define nng_sub_open nng_sub0_open
#endif

#ifndef nng_sub_open_raw
#define nng_sub_open_raw nng_sub0_open_raw
#endif

#define NNG_OPT_SUB_SUBSCRIBE "sub:subscribe"
#define NNG_OPT_SUB_UNSUBSCRIBE "sub:unsubscribe"

#define NNG_OPT_SUB_PREFNEW "sub:prefnew"

#ifdef __cplusplus
}
#endif

#endif // NNG_PROTOCOL_PUBSUB0_SUB_H
