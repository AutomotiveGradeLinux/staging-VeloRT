//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.
#include <sys/types.h>

#define CMD_BUFF_LEN 1024
extern char *cmd_output_buff;
extern int   cmd_output_len;

#ifndef NNG_PLATFORM_WINDOWS

#define CMD_RUN(cmd)                \
	do {                        \
		ret = cmd_run(cmd); \
		if (ret < 0)        \
			goto err;   \
	} while (0)

#define CMD_FRUN(fcmd, ...)                             \
	do {                                            \
		ret = nano_cmd_frun(fcmd, __VA_ARGS__); \
		if (ret < 0)                            \
			goto err;                       \
	} while (0)

extern int nano_cmd_frun(const char *format, ...);

#endif

extern int nano_cmd_run(const char *cmd);
extern int nano_cmd_run_status(const char *cmd);
extern void nano_cmd_cleanup(void);
