//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.
#ifndef VSOMEIP_GATEWAY_H
#define VSOMEIP_GATEWAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nng/supplemental/nanolib/conf.h"

extern int vsomeip_gateway(vsomeip_gateway_conf *conf);
extern int vsomeip_gateway_start(int argc, char **argv);
extern int vsomeip_gateway_dflt(int argc, char **argv);

#ifdef __cplusplus
}
#endif

#endif