//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <nng/compat/nanomsg/nn.h>
#include <nng/compat/nanomsg/pair.h>
#include "compat_testutil.h"

/*  This test checks whether blocking on send/recv works as expected. */

#define SOCKET_ADDRESS "inproc://a"

int sc;
int sb;

void worker (NN_UNUSED void *arg)
{
    /*  Wait 0.1 sec for the main thread to block. */
    nn_sleep (100);

    test_send (sc, "ABC");

    /*  Wait 0.1 sec for the main thread to process the previous message
        and block once again. */
    nn_sleep (100);

    test_send (sc, "ABC");
}

int main ()
{
    struct nn_thread thread;

    sb = test_socket (AF_SP, NN_PAIR);
    test_bind (sb, SOCKET_ADDRESS);
    sc = test_socket (AF_SP, NN_PAIR);
    test_connect (sc, SOCKET_ADDRESS);

    nn_thread_init (&thread, worker, NULL);

    test_recv (sb, "ABC");
    test_recv (sb, "ABC");

    nn_thread_term (&thread);

    test_close (sc);
    test_close (sb);

    return 0;
}

