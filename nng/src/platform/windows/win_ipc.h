//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef PLATFORM_WIN_WINIPC_H
#define PLATFORM_WIN_WINIPC_H

// This header file is private to the IPC (named pipes) support for Windows.

#include "core/nng_impl.h"
#include "win_impl.h"

#define IPC_PIPE_PREFIX "\\\\.\\pipe\\"

extern int nni_win_ipc_init(nng_stream **, HANDLE, const nng_sockaddr *, bool);

#endif // NNG_PLATFORM_WIN_WINIPC_H
