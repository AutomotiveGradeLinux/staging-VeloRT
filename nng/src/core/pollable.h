//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef CORE_POLLABLE_H
#define CORE_POLLABLE_H

#include "core/defs.h"
#include "core/list.h"

typedef struct nni_pollable nni_pollable;

extern void nni_pollable_raise(nni_pollable *);
extern void nni_pollable_clear(nni_pollable *);
extern int  nni_pollable_getfd(nni_pollable *, int *);

// nni_pollable implementation details are private.  Only here for inlining.
// We have joined the write and read file descriptors into a single
// atomic 64, so we can update them together (and we can use cas to be sure
// that such updates are always safe.)
struct nni_pollable {
	nni_atomic_u64  p_fds;
	nni_atomic_bool p_raised;
};

extern void nni_pollable_init(nni_pollable *);
extern void nni_pollable_fini(nni_pollable *);

#endif // CORE_POLLABLE_H
