//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.
#ifndef _NNG_SUPPLEMENTAL_NANOLIB_UTILS_H
#define _NNG_SUPPLEMENTAL_NANOLIB_UTILS_H

#include "nng/nng.h"
#include <ctype.h>
#include <stdio.h>

NNG_DECL void fatal(const char *msg, ...);
NNG_DECL void nng_fatal(const char *msg, int rv);
NNG_DECL uint16_t nano_pipe_get_local_port(nng_pipe p);
NNG_DECL uint16_t nano_pipe_get_local_port6(nng_pipe p);
#endif
