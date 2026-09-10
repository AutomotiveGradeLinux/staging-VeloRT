//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <nng/compat/nanomsg/nn.h>
#include <nng/compat/nanomsg/pair.h>
#include "compat_testutil.h"

int main (NN_UNUSED int argc, NN_UNUSED const char *argv[])
{
    int sb;
    int sc1;
    int sc2;

    sb = test_socket (AF_SP, NN_PAIR);
    test_bind (sb, "inproc://pair");
    sc1 = test_socket (AF_SP, NN_PAIR);
    test_connect (sc1, "inproc://pair");
    nn_sleep (100);
    sc2 = test_socket (AF_SP, NN_PAIR);
    test_connect (sc2, "inproc://pair");

    test_send (sb, "HELLO");
    test_recv (sc1, "HELLO");

    test_send (sc1, "THERE");
    test_recv (sb, "THERE");
    return 0;
}
