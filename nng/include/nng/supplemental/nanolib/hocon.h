//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.
#ifndef HOCON_H
#define HOCON_H
#include "cJSON.h"

extern cJSON *hocon_parse_file(const char *file);
extern void *hocon_parse_str(char *str, size_t len);


#endif