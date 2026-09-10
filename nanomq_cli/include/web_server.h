//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.
#ifndef __NANOMQ_CLI_WEB_SERVER_H__
#define __NANOMQ_CLI_WEB_SERVER_H__

#include <ctype.h>
#include <nng/nng.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "proxy.h"

#define HTTP_DEFAULT_USER "admin"
#define HTTP_DEFAULT_PASSWORD "public"
#define HTTP_DEFAULT_PORT 8082

#ifdef __cplusplus
extern "C" {
#endif

extern int  start_rest_server(proxy_info *proxy);
extern void stop_rest_server(void);

#ifdef __cplusplus
}
#endif


#endif // __NANOMQ_CLI_WEB_SERVER_H__