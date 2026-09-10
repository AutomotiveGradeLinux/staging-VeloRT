//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <nng/compat/nanomsg/nn.h>
#include <nng/compat/nanomsg/pair.h>

#include "compat_testutil.h"

#define SOCKET_ADDRESS "inproc://a"

int main ()
{
    int sb;
    int sc;

    sb = test_socket (AF_SP, NN_PAIR);
    test_bind (sb, SOCKET_ADDRESS);
    sc = test_socket (AF_SP, NN_PAIR);
    test_connect (sc, SOCKET_ADDRESS);

    test_send (sc, "ABC");
    test_recv (sb, "ABC");
    test_send (sb, "DEF");
    test_recv (sc, "DEF");

    test_close (sc);
    test_close (sb);

    return 0;
}

