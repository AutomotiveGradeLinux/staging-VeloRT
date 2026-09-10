//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#include "core/nng_impl.h"

#include "http_api.h"

static struct {
	const char *upper;
	const char *lower;
} http_schemes[] = {
	{
	    .upper = "http",
	    .lower = "tcp",
	},
	{
	    .upper = "ws",
	    .lower = "tcp",
	},
	{
	    .upper = "nmq-ws",
	    .lower = "tcp",
	},
	{
	    .upper = "nmq+ws",
	    .lower = "tcp",
	},
	{
	    .upper = "https",
	    .lower = "tls+tcp",
	},
	{
	    .upper = "wss",
	    .lower = "tls+tcp",
	},
	{
	    .upper = "nmq-wss",
	    .lower = "tls+tcp",
	},
	{
	    .upper = "nmq+wss",
	    .lower = "tls+tcp",
	},
	{
	    .upper = "http4",
	    .lower = "tcp4",
	},
	{
	    .upper = "ws4",
	    .lower = "tcp4",
	},
	{
	    .upper = "nmq-ws4",
	    .lower = "tcp4",
	},
	{
	    .upper = "nmq+ws4",
	    .lower = "tcp4",
	},
	{
	    .upper = "http6",
	    .lower = "tcp6",
	},
	{
	    .upper = "ws6",
	    .lower = "tcp6",
	},
	{
	    .upper = "nmq-ws6",
	    .lower = "tcp6",
	},
	{
	    .upper = "nmq+ws6",
	    .lower = "tcp6",
	},
	{
	    .upper = "https4",
	    .lower = "tls+tcp4",
	},
	{
	    .upper = "wss4",
	    .lower = "tls+tcp4",
	},
	{
	    .upper = "https6",
	    .lower = "tls+tcp6",
	},
	{
	    .upper = "wss6",
	    .lower = "tls+tcp6",
	},
	{
	    .upper = NULL,
	    .lower = NULL,
	},
};

const char *
nni_http_stream_scheme(const char *upper)
{
	for (int i = 0; http_schemes[i].upper != NULL; i++) {
		if (strcmp(http_schemes[i].upper, upper) == 0) {
			return (http_schemes[i].lower);
		}
	}
	return (NULL);
}
