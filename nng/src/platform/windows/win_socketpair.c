//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "core/nng_impl.h"


#ifdef NNG_HAVE_SOCKETPAIR_TODO
// TODO: Windows lacks socketpair.  We can emulate it with an explcit
// implementation based on AF_UNIX.

#include <errno.h>
#include <sys/socket.h>

int
nni_socket_pair(int fds[2])
{
	int rv;
	rv = socketpair(PF_UNIX, SOCK_STREAM, 0, fds);
	if (rv != 0) {
		return (nni_plat_errno(errno));
	}

	return (0);
}
#else
int
nni_socket_pair(int fds[2])
{
	NNI_ARG_UNUSED(fds);
	return (NNG_ENOTSUP);
}

// This is also the fdc transport.

typedef struct nni_sfd_conn nni_sfd_conn;

void
nni_sfd_close_fd(int fd)
{
	NNI_ARG_UNUSED(fd);
}

int
nni_sfd_conn_alloc(nni_sfd_conn **cp, int fd)
{
	NNI_ARG_UNUSED(cp);
	NNI_ARG_UNUSED(fd);
	return (NNG_ENOTSUP);
}

#endif
