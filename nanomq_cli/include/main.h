//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.
#ifndef NANOMQ_CLI_MAIN_H
#define NANOMQ_CLI_MAIN_H

#include "bench.h"
#include "client.h"
#include "nng_proxy.h"
#include "nngcat.h"
#include "zmq_gateway.h"
#include "vsomeip_gateway.h"
#include "rule_ctrl.h"
#include "dds_client.h"
#include "nftp_client.h"
#include "iceoryx_client.h"

#if defined(ENABLE_FILETRANSFER)
#include "file_transfer.h"
#endif

#endif // NANOMQ_CLI_MAIN_H
