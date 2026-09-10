//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef CORE_LISTENER_H
#define CORE_LISTENER_H

extern int      nni_listener_find(nni_listener **, uint32_t);
extern int      nni_listener_hold(nni_listener *);
extern void     nni_listener_rele(nni_listener *);
extern uint32_t nni_listener_id(nni_listener *);
extern int      nni_listener_create(nni_listener **, nni_sock *, const char *);
extern void     nni_listener_close(nni_listener *);
extern int      nni_listener_start(nni_listener *, int);
extern nni_sock *nni_listener_sock(nni_listener *);

extern int nni_listener_setopt(
    nni_listener *, const char *, const void *, size_t, nni_type);
extern int nni_listener_getopt(
    nni_listener *, const char *, void *, size_t *, nni_type);
extern const nng_url *nni_listener_url(nni_listener *);
extern void nni_listener_add_stat(nni_listener *, nni_stat_item *);
extern void nni_listener_bump_error(nni_listener *, int);

#endif // CORE_LISTENER_H
