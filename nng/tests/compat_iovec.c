//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <nng/compat/nanomsg/nn.h>
#include <nng/compat/nanomsg/pair.h>

#include "compat_testutil.h"

#include <string.h>

#define SOCKET_ADDRESS "inproc://a"

int main ()
{
    int rc;
    int sb;
    int sc;
    struct nn_iovec iov [2];
    struct nn_msghdr hdr;
    char buf [6];

    sb = test_socket (AF_SP, NN_PAIR);
    test_bind (sb, SOCKET_ADDRESS);
    sc = test_socket (AF_SP, NN_PAIR);
    test_connect (sc, SOCKET_ADDRESS);

    iov [0].iov_base = "AB";
    iov [0].iov_len = 2;
    iov [1].iov_base = "CDEF";
    iov [1].iov_len = 4;
    memset (&hdr, 0, sizeof (hdr));
    hdr.msg_iov = iov;
    hdr.msg_iovlen = 2;
    rc = nn_sendmsg (sc, &hdr, 0);
    errno_assert (rc >= 0);
    nn_assert (rc == 6);

    iov [0].iov_base = buf;
    iov [0].iov_len = 4;
    iov [1].iov_base = buf + 4;
    iov [1].iov_len = 2;
    memset (&hdr, 0, sizeof (hdr));
    hdr.msg_iov = iov;
    hdr.msg_iovlen = 2;
    rc = nn_recvmsg (sb, &hdr, 0);
    errno_assert (rc >= 0);
    nn_assert (rc == 6);
    nn_assert (memcmp (buf, "ABCDEF", 6) == 0);

    test_close (sc);
    test_close (sb);

    return 0;
}

