//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.
#ifndef NANO_EXTERN_FILETRANS
#define NANO_EXTERN_FILETRANS

#define MD5_LEN 32

int CalcFileMD5(char *file_name, char *md5_sum);
int CalcMD5n(char *binary, size_t len, char *tmpfpath, char **md5res);

int file_transfer(int argc, char **argv);

#endif
