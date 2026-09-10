//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.
#ifndef NANOMQ_ACL_HANDLER_H
#define NANOMQ_ACL_HANDLER_H

#include "nng/nng.h"
#include "nng/supplemental/nanolib/conf.h"
#include "nng/supplemental/nanolib/acl_conf.h"

#ifdef ACL_SUPP
extern bool auth_acl(
    conf *config, acl_action_type type, conn_param *param, const char *topic);
#endif
#endif
