//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.
#ifndef DB_CLI_H
#define DB_CLI_H

#include "fdb_version.h"
#include <pthread.h>
#include <stdlib.h>

#include <foundationdb/fdb_c.h>
#include <foundationdb/fdb_c_options.g.h>

FDBDatabase* openDatabase(pthread_t* netThread);
#endif