#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
nng_directory(core)

nng_check_sym(strlcpy string.h NNG_HAVE_STRLCPY)
nng_check_sym(strnlen string.h NNG_HAVE_STRNLEN)
nng_check_sym(strcasecmp string.h NNG_HAVE_STRCASECMP)
nng_check_sym(strncasecmp string.h NNG_HAVE_STRNCASECMP)

nng_sources(
   mqtt_public.c
   mqtt_codec.c
   mqtt_msg.c
   mqtt_msg.h 
   mqtt_qos_db_api.c
   mqtt_qos_db_api.h
)

nng_test(mqtt_test)

nng_sources_if(NNG_ENABLE_SQLITE  
   mqtt_qos_db.c 
   mqtt_qos_db.h
)

nng_test_if(NNG_ENABLE_SQLITE mqtt_qos_db_test)
