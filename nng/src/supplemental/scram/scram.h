//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef NNG_SUPP_SCRAM_H
#define NNG_SUPP_SCRAM_H

enum SCRAM_digest {
	SCRAM_SHA1,
	SCRAM_SHA256
};

void *scram_ctx_create(char *pwd, int pwdsz, int it_cnt, enum SCRAM_digest digest, int salt);
void  scram_ctx_free(void *ctx);

char *scram_client_first_msg(void *arg, const char *username);

char *scram_handle_client_first_msg(void *arg, const char *msg, int len);
char *scram_handle_server_first_msg(void *arg, const char *msg, int len);
char *scram_handle_client_final_msg(void *arg, const char *msg, int len);
char *scram_handle_server_final_msg(void *arg, const char *msg, int len);

#endif
