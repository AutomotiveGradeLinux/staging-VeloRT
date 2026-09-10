//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef NNI_BASE64_INCLUDED
#define NNI_BASE64_INCLUDED

#include <stddef.h>
#include <stdint.h>

// Based on base64.c (Public Domain) by Jon Mayo.
// Base64 is defined in RFC 2045, section 6.8.

// This function encodes an arbitrary byte array into base64
// null-terminated string.  It returns the number of characters
// emitted.
size_t nni_base64_encode(const uint8_t *, size_t, char *, size_t);

// This function decodes a base64 string into supplied buffer.
// It returns the number of bytes emitted.
size_t nni_base64_decode(const char *, size_t, uint8_t *, size_t);

#endif
