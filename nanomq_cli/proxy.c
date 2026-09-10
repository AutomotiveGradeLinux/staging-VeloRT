//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.
#include "include/proxy.h"

proxy_info *
proxy_info_alloc(const char *name, void *conf, char *conf_path,
    conf_http_server *http_conf, int argc, char **argv)
{
	proxy_info *info  = nng_zalloc(sizeof(proxy_info));
	info->proxy_name  = name;
	info->conf        = conf;
	info->conf_path   = conf_path;
	info->http_server = http_conf;
	info->args.argc   = argc;
	info->args.argv   = argv;

	return info;
}