
//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "nng/supplemental/nanolib/nmq_base64.h"
#include "supplemental/base64/base64.h"
#include <nng/nng.h>

size_t
nmq_base64_encode(const uint8_t *in, size_t in_len, char *out, size_t out_len)
{
	return nni_base64_encode(in, in_len, out, out_len);
}

size_t
nmq_base64_decode(const char *in, size_t in_len, uint8_t *out, size_t out_len)
{
	return nni_base64_decode(in, in_len, out, out_len);
}