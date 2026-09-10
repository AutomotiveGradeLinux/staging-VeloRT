//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.
#ifndef RAW_STREAM_H
#define RAW_STREAM_H

#include "nng/exchange/stream/stream.h"
#define RAW_STREAM_NAME "raw"
#define RAW_STREAM_ID 0

int raw_stream_register();
void *raw_cmd_parser(void *);
void *raw_decode(void *);
void *raw_encode(void *);

#endif // RAW_STREAM_H
