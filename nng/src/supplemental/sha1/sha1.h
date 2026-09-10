//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef NNG_SUPPLEMENTAL_SHA1_SHA1_H
#define NNG_SUPPLEMENTAL_SHA1_SHA1_H

typedef struct {
	uint32_t digest[5]; // resulting digest
	uint64_t len;       // length in bits
	uint8_t  blk[64];   // message block
	int      idx;       // index of next byte in block
} nni_sha1_ctx;

extern void nni_sha1_init(nni_sha1_ctx *);
extern void nni_sha1_update(nni_sha1_ctx *, const void *, size_t);
extern void nni_sha1_final(nni_sha1_ctx *, uint8_t[20]);
extern void nni_sha1(const void *, size_t, uint8_t[20]);

#endif // NNG_SUPPLEMENTAL_SHA1_SHA1_H
