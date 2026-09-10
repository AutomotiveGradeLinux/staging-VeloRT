//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <nng/compat/nanomsg/nn.h>
#include <nng/compat/nanomsg/pipeline.h>
#include "compat_testutil.h"

#define SOCKET_ADDRESS "inproc://a"

int main ()
{
    int push1;
    int push2;
    int pull1;
    int pull2;

    /*  Test fan-out. */

    push1 = test_socket (AF_SP, NN_PUSH);
    test_bind (push1, SOCKET_ADDRESS);
    pull1 = test_socket (AF_SP, NN_PULL);
    test_connect (pull1, SOCKET_ADDRESS);
    nn_sleep (20);
    pull2 = test_socket (AF_SP, NN_PULL);
    test_connect (pull2, SOCKET_ADDRESS);

    /*  Wait till both connections are established to get messages spread
        evenly between the two pull sockets. */
    nn_sleep (20);

    test_send (push1, "ABC");
    test_send (push1, "DEF");

    test_recv (pull1, "ABC");
    test_recv (pull2, "DEF");

    test_close (push1);
    test_close (pull1);
    test_close (pull2);

    /*  Test fan-in. */

    pull1 = test_socket (AF_SP, NN_PULL);
    test_bind (pull1, SOCKET_ADDRESS);
    push1 = test_socket (AF_SP, NN_PUSH);
    test_connect (push1, SOCKET_ADDRESS);
    push2 = test_socket (AF_SP, NN_PUSH);
    test_connect (push2, SOCKET_ADDRESS);

    nn_sleep (10);
    test_send (push1, "ABC");

    nn_sleep (10);
    test_send (push2, "DEF");

    test_recv (pull1, "ABC");
    test_recv (pull1, "DEF");

    test_close (pull1);
    test_close (push1);
    test_close (push2);

    return 0;
}

