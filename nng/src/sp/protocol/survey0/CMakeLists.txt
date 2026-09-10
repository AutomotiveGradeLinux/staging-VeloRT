#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
nng_directory(survey0)

nng_sources_if(NNG_PROTO_SURVEYOR0 survey.c xsurvey.c)
nng_headers_if(NNG_PROTO_SURVEYOR0 nng/protocol/survey0/survey.h)
nng_defines_if(NNG_PROTO_SURVEYOR0 NNG_HAVE_SURVEYOR0)

nng_sources_if(NNG_PROTO_RESPONDENT0 respond.c xrespond.c)
nng_headers_if(NNG_PROTO_RESPONDENT0 nng/protocol/survey0/respond.h)
nng_defines_if(NNG_PROTO_RESPONDENT0 NNG_HAVE_RESPONDENT0)

nng_test(respond_test)
nng_test(survey_test)
nng_test(xrespond_test)
nng_test(xsurvey_test)