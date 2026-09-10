//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef NNG_COMPAT_PIPELINE_H
#define NNG_COMPAT_PIPELINE_H

// This header contains interfaces that are intended to offer compatibility
// with nanomsg v1.0.  These are not the "preferred" interfaces for nng,
// and consumers should only use these if they are porting software that
// previously used nanomsg.  New programs should use the nng native APIs.

#ifdef __cplusplus
extern "C" {
#endif

// PUSH and PULL sockopt level.
#define NN_PROTO_PIPELINE 5
#define NN_PUSH (NN_PROTO_PIPELINE * 16 + 0)
#define NN_PULL (NN_PROTO_PIPELINE * 16 + 1)

// PUSH and PULL have no options.

#ifdef __cplusplus
}
#endif

#endif // NNG_COMPAT_PIPELINE_H
