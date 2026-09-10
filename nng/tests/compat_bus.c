//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <nng/compat/nanomsg/nn.h>
#include <nng/compat/nanomsg/bus.h>
#include "compat_testutil.h"

#define SOCKET_ADDRESS_A "inproc://a"
#define SOCKET_ADDRESS_B "inproc://b"

int main ()
{
    int rc;
    int bus1;
    int bus2;
    int bus3;
    char buf [3];

    /*  Create a simple bus topology consisting of 3 nodes. */
    bus1 = test_socket (AF_SP, NN_BUS);
    test_bind (bus1, SOCKET_ADDRESS_A);
    bus2 = test_socket (AF_SP, NN_BUS);
    test_bind (bus2, SOCKET_ADDRESS_B);

    nn_sleep(100);

    test_connect (bus2, SOCKET_ADDRESS_A);
    bus3 = test_socket (AF_SP, NN_BUS);
    test_connect (bus3, SOCKET_ADDRESS_A);
    test_connect (bus3, SOCKET_ADDRESS_B);

    /* Wait for connections to establish asynchronously. */
    nn_sleep(100);

    /*  Send a message from each node. */
    test_send (bus1, "A");
    test_send (bus2, "AB");
    test_send (bus3, "ABC");

    /*  Check that two messages arrived at each node. */
    rc = nn_recv (bus1, buf, 3, 0);
    errno_assert (rc >= 0);
    nn_assert (rc == 2 || rc == 3);
    rc = nn_recv (bus1, buf, 3, 0);
    errno_assert (rc >= 0);
    nn_assert (rc == 2 || rc == 3);
    rc = nn_recv (bus2, buf, 3, 0);
    errno_assert (rc >= 0);
    nn_assert (rc == 1 || rc == 3);
    rc = nn_recv (bus2, buf, 3, 0);
    errno_assert (rc >= 0);
    nn_assert (rc == 1 || rc == 3);
    rc = nn_recv (bus3, buf, 3, 0);
    errno_assert (rc >= 0);
    nn_assert (rc == 1 || rc == 2);
    rc = nn_recv (bus3, buf, 3, 0);
    errno_assert (rc >= 0);
    nn_assert (rc == 1 || rc == 2);

    /*  Wait till both connections are established. */
    nn_sleep (10);

    test_close (bus3);
    test_close (bus2);
    test_close (bus1);

    return 0;
}

