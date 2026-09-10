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

char longdata[1 << 20];

int main (int argc, const char *argv[])
{
    int rc;
    int sb;
    int sc;
    unsigned char *buf1, *buf2;
    int i;
    struct nn_iovec iov;
    struct nn_msghdr hdr;
    char socket_address_tcp[128];

    test_addr_from(socket_address_tcp, "tcp", "127.0.0.1",
            get_test_port(argc, argv));

    sb = test_socket (AF_SP, NN_PAIR);
    test_bind (sb, SOCKET_ADDRESS);
    sc = test_socket (AF_SP, NN_PAIR);
    test_connect (sc, SOCKET_ADDRESS);

    buf1 = nn_allocmsg (256, 0);
    alloc_assert (buf1);
    for (i = 0; i != 256; ++i)
        buf1 [i] = (unsigned char) i;
    rc = nn_send (sc, &buf1, NN_MSG, 0);
    errno_assert (rc >= 0);
    nn_assert (rc == 256);

    buf2 = NULL;
    rc = nn_recv (sb, &buf2, NN_MSG, 0);
    errno_assert (rc >= 0);
    nn_assert (rc == 256);
    nn_assert (buf2);
    for (i = 0; i != 256; ++i)
        nn_assert (buf2 [i] == (unsigned char) i);
    rc = nn_freemsg (buf2);
    errno_assert (rc == 0);

    buf1 = nn_allocmsg (256, 0);
    alloc_assert (buf1);
    for (i = 0; i != 256; ++i)
        buf1 [i] = (unsigned char) i;
    iov.iov_base = &buf1;
    iov.iov_len = NN_MSG;
    memset (&hdr, 0, sizeof (hdr));
    hdr.msg_iov = &iov;
    hdr.msg_iovlen = 1;
    rc = nn_sendmsg (sc, &hdr, 0);
    errno_assert (rc >= 0);
    nn_assert (rc == 256);

    buf2 = NULL;
    iov.iov_base = &buf2;
    iov.iov_len = NN_MSG;
    memset (&hdr, 0, sizeof (hdr));
    hdr.msg_iov = &iov;
    hdr.msg_iovlen = 1;
    rc = nn_recvmsg (sb, &hdr, 0);
    errno_assert (rc >= 0);
    nn_assert (rc == 256);
    nn_assert (buf2);
    for (i = 0; i != 256; ++i)
        nn_assert (buf2 [i] == (unsigned char) i);
    rc = nn_freemsg (buf2);
    errno_assert (rc == 0);

    test_close (sc);
    test_close (sb);

    /*  Test receiving of large message  */

    sb = test_socket (AF_SP, NN_PAIR);
    test_bind (sb, socket_address_tcp);
    sc = test_socket (AF_SP, NN_PAIR);
    test_connect (sc, socket_address_tcp);

    for (i = 0; i < (int) sizeof (longdata); ++i)
        longdata[i] = '0' + (i % 10);
    longdata [sizeof (longdata) - 1] = 0;
    test_send (sb, longdata);

    rc = nn_recv (sc, &buf2, NN_MSG, 0);
    errno_assert (rc >= 0);
    nn_assert (rc == sizeof (longdata) - 1);
    nn_assert (buf2);
    for (i = 0; i < (int) sizeof (longdata) - 1; ++i)
        nn_assert (buf2 [i] == longdata [i]);
    rc = nn_freemsg (buf2);
    errno_assert (rc == 0);

    test_close (sc);
    test_close (sb);

    return 0;
}

