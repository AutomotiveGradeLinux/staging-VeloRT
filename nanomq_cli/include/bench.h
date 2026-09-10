//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.
#ifndef NANOMQ_BENCH_H
#define NANOMQ_BENCH_H

#if !defined(NANO_PLATFORM_WINDOWS) && defined(SUPP_BENCH)
//TODO support windows later
extern int bench_start(int argc, char **argv);
extern int bench_dflt(int argc, char **argv);
#endif

#endif //NANOMQ_BENCH_H