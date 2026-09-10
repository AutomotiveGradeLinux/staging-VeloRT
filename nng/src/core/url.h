//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef CORE_URL_H
#define CORE_URL_H

#include "core/defs.h"

extern int         nni_url_parse(nni_url **, const char *path);
extern void        nni_url_free(nni_url *);
extern int         nni_url_clone(nni_url **, const nni_url *);
extern const char *nni_url_default_port(const char *);
extern int         nni_url_asprintf(char **, const nni_url *);
extern int         nni_url_asprintf_port(char **, const nni_url *, int);
extern size_t      nni_url_decode(uint8_t *, const char *, size_t);
extern int         nni_url_to_address(nng_sockaddr *, const nni_url *);

#endif // CORE_URL_H
