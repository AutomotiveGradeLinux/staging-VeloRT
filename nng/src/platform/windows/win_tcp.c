//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "core/nng_impl.h"

#ifdef NNG_PLATFORM_WINDOWS

#include <malloc.h>
#include <stdio.h>

int
nni_win_tcp_sysinit(void)
{
	WSADATA data;
	if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
		NNI_ASSERT(LOBYTE(data.wVersion) == 2);
		NNI_ASSERT(HIBYTE(data.wVersion) == 2);
		return (nni_win_error(GetLastError()));
	}
	return (0);
}

void
nni_win_tcp_sysfini(void)
{
	WSACleanup();
}

#endif // NNG_PLATFORM_WINDOWS
