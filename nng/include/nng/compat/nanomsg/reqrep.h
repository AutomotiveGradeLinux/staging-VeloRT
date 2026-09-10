//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef NNG_COMPAT_REQREP_H
#define NNG_COMPAT_REQREP_H

// This header contains interfaces that are intended to offer compatibility
// with nanomsg v1.0.  These are not the "preferred" interfaces for nng,
// and consumers should only use these if they are porting software that
// previously used nanomsg.  New programs should use the nng native APIs.

#ifdef __cplusplus
extern "C" {
#endif

// REQ and REP sockopt level.
#define NN_PROTO_REQREP 3
#define NN_REQ (NN_PROTO_REQREP * 16 + 0)
#define NN_REP (NN_PROTO_REQREP * 16 + 1)

// REQ options.  (REP has none.)
#define NN_REQ_RESEND_IVL (NN_REQ * 16 + 1)

#ifdef __cplusplus
}
#endif

#endif // NNG_COMPAT_REQREP_H
