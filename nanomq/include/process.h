//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.
#ifndef PROCESS_H
#define PROCESS_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

extern int process_is_alive(int pid);
extern int process_send_signal(int pid, int signal);
extern int pidgrp_send_signal(int pid, int signal);
extern int process_daemonize(void);
extern int process_create_child(int (*child_run)(void *), void *data);

#endif