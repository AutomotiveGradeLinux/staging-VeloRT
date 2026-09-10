//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.
#ifndef NANO_EXTERN_AES_GCM
#define NANO_EXTERN_AES_GCM

char *aes_gcm_decrypt(char *cipher, int cipher_len, char *key, int *plain_lenp);
char *aes_gcm_encrypt(char *plain, int plain_len, char *key, int *cipher_lenp);

#endif
