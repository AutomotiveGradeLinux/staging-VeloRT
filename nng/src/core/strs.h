//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef CORE_STRS_H
#define CORE_STRS_H

// Safe string functions, in case the platform misses these.

extern char * nni_strdup(const char *);
extern char * nni_strndup(const char *, size_t);
extern char * nni_strnins(char *, const char *, size_t , size_t);
extern char * nni_strncat(char *, const char *, size_t , size_t);
extern void   nni_strfree(char *);
extern size_t nni_strlcpy(char *, const char *, size_t);
extern size_t nni_strnlen(const char *, size_t);
extern char * nni_strcasestr(const char *, const char *);
extern int    nni_strncasecmp(const char *, const char *, size_t);
extern int    nni_strcasecmp(const char *, const char *);
extern int    nni_asprintf(char **, const char *, ...);

#endif // CORE_STRS_H
