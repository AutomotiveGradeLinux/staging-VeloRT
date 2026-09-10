//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <nng/compat/nanomsg/nn.h>
#include <nng/compat/nanomsg/bus.h>
#include <nng/compat/nanomsg/pair.h>
#include <nng/compat/nanomsg/pipeline.h>
#include <nng/compat/nanomsg/inproc.h>
#include "compat_testutil.h"

#define SOCKET_ADDRESS_A    "inproc://a"
#define SOCKET_ADDRESS_B    "inproc://b"
#define SOCKET_ADDRESS_C    "inproc://c"
#define SOCKET_ADDRESS_D    "inproc://d"
#define SOCKET_ADDRESS_E    "inproc://e"

int deva = -1;
int devb = -1;
int devc = -1;
int devd = -1;
int deve = -1;

void device1(NN_UNUSED void *arg)
{
    int rc;

    /*  Intialise the device sockets. */
    deva = test_socket(AF_SP_RAW, NN_PAIR);
    test_bind(deva, SOCKET_ADDRESS_A);
    devb = test_socket(AF_SP_RAW, NN_PAIR);
    test_bind(devb, SOCKET_ADDRESS_B);

    /*  Run the device. */
    rc = nn_device(deva, devb);
    nn_assert(rc < 0 && (nn_errno() == EBADF));
}


void device2(NN_UNUSED void *arg)
{
    int rc;

    /*  Intialise the device sockets. */
    devc = test_socket(AF_SP_RAW, NN_PULL);
    test_bind(devc, SOCKET_ADDRESS_C);
    devd = test_socket(AF_SP_RAW, NN_PUSH);
    test_bind(devd, SOCKET_ADDRESS_D);

    /*  Run the device. */
    rc = nn_device(devc, devd);
    nn_assert(rc < 0 && nn_errno() == EBADF);
}


void device3(NN_UNUSED void *arg)
{
    int rc;

    /*  Intialise the device socket. */
    deve = test_socket(AF_SP_RAW, NN_BUS);
    test_bind(deve, SOCKET_ADDRESS_E);

    /*  Run the device. */
    rc = nn_device(deve, -1);
    nn_assert(rc < 0 && nn_errno() == EBADF);
}


int main()
{
    int enda;
    int endb;
    int endc;
    int endd;
    int ende1;
    int ende2;
    struct nn_thread thread1;
    struct nn_thread thread2;
    struct nn_thread thread3;
    int timeo;

    /*  Test the bi-directional device. */

    /*  Start the device. */
    nn_thread_init(&thread1, device1, NULL);

    /*  Create two sockets to connect to the device. */
    enda = test_socket(AF_SP, NN_PAIR);
    test_connect(enda, SOCKET_ADDRESS_A);
    endb = test_socket(AF_SP, NN_PAIR);
    test_connect(endb, SOCKET_ADDRESS_B);

    nn_sleep(100);

    /*  Pass a pair of messages between endpoints. */
    test_send(enda, "ABC");
    test_recv(endb, "ABC");
    test_send(endb, "ABC");
    test_recv(enda, "ABC");

    /*  Clean up. */
    test_close(endb);
    test_close(enda);
    test_close(deva);
    test_close(devb);

    /*  Test the uni-directional device. */

    /*  Start the device. */
    nn_thread_init(&thread2, device2, NULL);

    nn_sleep(100);

    /*  Create two sockets to connect to the device. */
    endc = test_socket(AF_SP, NN_PUSH);
    test_connect(endc, SOCKET_ADDRESS_C);
    endd = test_socket(AF_SP, NN_PULL);
    test_connect(endd, SOCKET_ADDRESS_D);


    /*  Pass a message between endpoints. */
    test_send(endc, "XYZ");
    test_recv(endd, "XYZ");

    /*  Clean up. */
    test_close(endd);
    test_close(endc);
    test_close(devc);
    test_close(devd);

    /*  Test the loopback device. */

    /*  Start the device. */
    nn_thread_init(&thread3, device3, NULL);
    nn_sleep(100);

    /*  Create two sockets to connect to the device. */
    ende1 = test_socket(AF_SP, NN_BUS);
    test_connect(ende1, SOCKET_ADDRESS_E);
    ende2 = test_socket(AF_SP, NN_BUS);
    test_connect(ende2, SOCKET_ADDRESS_E);

    /*  BUS is unreliable so wait a bit for connections to be established. */
    nn_sleep(200);

    /*  Pass a message to the bus. */
    test_send(ende1, "KLM");
    test_recv(ende2, "KLM");

    /*  Make sure that the message doesn't arrive at the socket it was
     *  originally sent to. */
    timeo = 100;
    test_setsockopt(ende1, NN_SOL_SOCKET, NN_RCVTIMEO,
        &timeo, sizeof (timeo));
    test_drop(ende1, ETIMEDOUT);

    /*  Clean up. */
    test_close(ende2);
    test_close(ende1);
    test_close(deve);

    /*  Shut down the devices. */
    nn_term();
    nn_thread_term(&thread1);
    nn_thread_term(&thread2);
    nn_thread_term(&thread3);

    return (0);
}
