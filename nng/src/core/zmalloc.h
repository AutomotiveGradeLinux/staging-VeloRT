//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef __ZMALLOC_H
#define __ZMALLOC_H
#include <stddef.h>

void * zmalloc(size_t size);
void * zrealloc(void *ptr, size_t size);
void   zfree(void *ptr);
char * zstrdup(const char *s);
size_t zmalloc_used_memory(void);

#endif /* _ZMALLOC_H */
