//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <nng/compat/nanomsg/nn.h>
#include <nng/compat/nanomsg/tcp.h>
#include <nng/compat/nanomsg/reqrep.h>
#include "compat_testutil.h"

int main (int argc, const char *argv[])
{
    int s;
    int rc;
    int eid;
    char socket_address[128];

    test_addr_from(socket_address, "tcp", "127.0.0.1",
            get_test_port(argc, argv));

    /*  Run endpoint shutdown and socket shutdown in parallel. */
    s = test_socket (AF_SP, NN_REQ);
    eid = test_connect (s, socket_address);
    rc = nn_shutdown (s, eid);
    errno_assert (rc == 0);
    test_close (s);

    return 0;
}

