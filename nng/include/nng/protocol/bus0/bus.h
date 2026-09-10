//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef NNG_PROTOCOL_BUS0_BUS_H
#define NNG_PROTOCOL_BUS0_BUS_H

#ifdef __cplusplus
extern "C" {
#endif

NNG_DECL int nng_bus0_open(nng_socket *);

NNG_DECL int nng_bus0_open_raw(nng_socket *);

#ifndef nng_bus_open
#define nng_bus_open nng_bus0_open
#endif

#ifndef nng_bus_open_raw
#define nng_bus_open_raw nng_bus0_open_raw
#endif

#define NNG_BUS0_SELF 0x70
#define NNG_BUS0_PEER 0x70
#define NNG_BUS0_SELF_NAME "bus"
#define NNG_BUS0_PEER_NAME "bus"

#ifdef __cplusplus
}
#endif

#endif // NNG_PROTOCOL_BUS0_BUS_H
