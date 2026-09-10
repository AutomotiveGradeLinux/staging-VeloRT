//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

// Note: This file started life in nanomsg.  We have copied it, and adjusted
// it for validating the compatibility features of nanomsg.   As much as
// possible we want to run tests from the nanomsg test suite unmodified.

#ifndef COMPAT_TESTUTIL_H_INCLUDED
#define COMPAT_TESTUTIL_H_INCLUDED

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define nn_err_strerror nn_strerror
#define nn_err_abort abort
#define errno_assert nn_assert
#define wsa_assert nn_assert
#define alloc_assert(x) nn_assert((x) != NULL)

#if defined __GNUC__ || defined __llvm__ || defined __clang__
#define NN_UNUSED __attribute__((unused))
#else
#define NN_UNUSED
#endif

#define nn_assert(x) nn_assert_impl(x, #x, __FILE__, __LINE__)

extern int  test_socket_impl(char *file, int line, int family, int protocol);
extern int  test_connect_impl(char *file, int line, int sock, char *address);
extern int  test_bind_impl(char *file, int line, int sock, char *address);
extern void test_close_impl(char *file, int line, int sock);
extern void test_send_impl(char *file, int line, int sock, char *data);
extern void test_recv_impl(char *file, int line, int sock, char *data);
extern void test_drop_impl(char *file, int line, int sock, int err);
extern int  test_setsockopt_impl(char *file, int line, int sock, int level,
     int option, const void *optval, size_t optlen);
extern int  get_test_port(int argc, const char *argv[]);
extern void test_addr_from(
    char *out, const char *proto, const char *ip, int port);
extern void nn_sleep(int);
extern void nn_assert_impl(
    bool b, const char *expression, const char *file, int line);

#define test_socket(f, p) test_socket_impl(__FILE__, __LINE__, (f), (p))
#define test_connect(s, a) test_connect_impl(__FILE__, __LINE__, (s), (a))
#define test_bind(s, a) test_bind_impl(__FILE__, __LINE__, (s), (a))
#define test_send(s, d) test_send_impl(__FILE__, __LINE__, (s), (d))
#define test_recv(s, d) test_recv_impl(__FILE__, __LINE__, (s), (d))
#define test_drop(s, e) test_drop_impl(__FILE__, __LINE__, (s), (e))
#define test_close(s) test_close_impl(__FILE__, __LINE__, (s))
#define test_setsockopt(s, l, o, v, z) \
	test_setsockopt_impl(__FILE__, __LINE__, (s), (l), (o), (v), (z))

struct nn_thread {
	void *thr;
};

extern int  nn_thread_init(struct nn_thread *, void (*)(void *), void *);
extern void nn_thread_term(struct nn_thread *);

#endif // COMPAT_TESTUTIL_H_INCLUDED
