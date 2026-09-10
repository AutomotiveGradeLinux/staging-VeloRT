//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef NNG_COMPAT_SURVEY_H
#define NNG_COMPAT_SURVEY_H

// This header contains interfaces that are intended to offer compatibility
// with nanomsg v1.0.  These are not the "preferred" interfaces for nng,
// and consumers should only use these if they are porting software that
// previously used nanomsg.  New programs should use the nng native APIs.

#ifdef __cplusplus
extern "C" {
#endif

// SURVEYOR and RESPONDENT sockopt level.
#define NN_PROTO_SURVEY 6
#define NN_SURVEYOR (NN_PROTO_SURVEY * 16 + 2)
#define NN_RESPONDENT (NN_PROTO_SURVEY * 16 + 3)

// SURVEYOR options.  (RESPONDENT has none.)

#define NN_SURVEYOR_DEADLINE (NN_SURVEYOR * 16 + 1)

#ifdef __cplusplus
}
#endif

#endif // NNG_COMPAT_SURVEY_H
