//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef CORE_DEVICE_H
#define CORE_DEVICE_H

// Device takes messages from one side, and forwards them to the other.
// It works in both directions.  Arguably we should build versions of this
// that are unidirectional, and we could extend this API with user-defined
// filtering functions.
extern void nni_device(nni_aio *aio, nni_sock *, nni_sock *);

#endif // CORE_DEVICE_H
