//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef NNG_TRANSPORT_IPC_IPC_H
#define NNG_TRANSPORT_IPC_IPC_H

#include <nng/nng.h>

#ifdef __cplusplus
extern "C" {
#endif

// ipc transport.  This is used for inter-process communication on
// the same host computer.

#ifndef NNG_ELIDE_DEPRECATED
NNG_DECL int nng_ipc_register(void);
#endif

#ifdef __cplusplus
}
#endif

#endif // NNG_TRANSPORT_IPC_IPC_H
