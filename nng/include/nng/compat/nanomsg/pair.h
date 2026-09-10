//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef NNG_COMPAT_PAIR_H
#define NNG_COMPAT_PAIR_H

// This header contains interfaces that are intended to offer compatibility
// with nanomsg v1.0.  These are not the "preferred" interfaces for nng,
// and consumers should only use these if they are porting software that
// previously used nanomsg.  New programs should use the nng native APIs.

#ifdef __cplusplus
extern "C" {
#endif

// PAIR sockopt level.
#define NN_PROTO_PAIR 1
#define NN_PAIR (NN_PROTO_PAIR * 16 + 0)

// These are technically "new", and not available in nanomsg, but
// offered here as a transition aid.  If you want to use the advanced
// PAIRv1 options (POLYAMOROUS mode) you still need to use the new API.
#define NN_PAIR_v0 (NN_PROTO_PAIR * 16 + 0)
#define NN_PAIR_V1 (NN_PROTO_PAIR * 16 + 1)

// PAIR has no options.

#ifdef __cplusplus
}
#endif

#endif // NNG_COMPAT_PAIR_H
