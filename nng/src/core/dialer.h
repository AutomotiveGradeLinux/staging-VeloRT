//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef CORE_DIALER_H
#define CORE_DIALER_H

extern int       nni_dialer_find(nni_dialer **, uint32_t);
extern int       nni_dialer_hold(nni_dialer *);
extern void      nni_dialer_rele(nni_dialer *);
extern uint32_t  nni_dialer_id(nni_dialer *);
extern int       nni_dialer_create(nni_dialer **, nni_sock *, const char *);
extern void      nni_dialer_close(nni_dialer *);
extern int       nni_dialer_start(nni_dialer *, unsigned);
extern nni_sock *nni_dialer_sock(nni_dialer *);

extern int nni_dialer_setopt(
    nni_dialer *, const char *, const void *, size_t, nni_type);
extern int nni_dialer_getopt(
    nni_dialer *, const char *, void *, size_t *, nni_type);
extern const nng_url *nni_dialer_url(nni_dialer *);
extern void nni_dialer_add_stat(nni_dialer *, nni_stat_item *);
extern void nni_dialer_bump_error(nni_dialer *, int);

// NNG-MQTT
extern void nni_dialer_setcb(nni_dialer *, void * cb);
extern void nni_dialer_off(nni_dialer *d);  // turn off auto connect & disconnect
#endif // CORE_DIALER_H
