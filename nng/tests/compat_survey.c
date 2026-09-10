//
// Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <nng/compat/nanomsg/nn.h>
#include <nng/compat/nanomsg/survey.h>

#include "compat_testutil.h"

#define SOCKET_ADDRESS "inproc://test"

int
main()
{
	int  rc;
	int  surveyor;
	int  respondent1;
	int  respondent2;
	int  respondent3;
	int  deadline;
	char buf[7];

	/*  Test a simple survey with three respondents. */
	surveyor = test_socket(AF_SP, NN_SURVEYOR);
	deadline = 500;
	rc       = nn_setsockopt(surveyor, NN_SURVEYOR, NN_SURVEYOR_DEADLINE,
	    &deadline, sizeof(deadline));
	errno_assert(rc == 0);
	test_bind(surveyor, SOCKET_ADDRESS);
	respondent1 = test_socket(AF_SP, NN_RESPONDENT);
	test_connect(respondent1, SOCKET_ADDRESS);
	respondent2 = test_socket(AF_SP, NN_RESPONDENT);
	test_connect(respondent2, SOCKET_ADDRESS);
	respondent3 = test_socket(AF_SP, NN_RESPONDENT);
	test_connect(respondent3, SOCKET_ADDRESS);

	/* Sleep a tiny bit. */
	nn_sleep(1000);

	/* Check that attempt to recv with no survey pending is EFSM. */
	rc = nn_recv(surveyor, buf, sizeof(buf), 0);
	errno_assert(rc == -1 && nn_errno() == EFSM);

	/*  Send the survey. */
	test_send(surveyor, "ABC");

	/*  First respondent answers. */
	test_recv(respondent1, "ABC");
	test_send(respondent1, "DEF");

	/*  Second respondent answers. */
	test_recv(respondent2, "ABC");
	test_send(respondent2, "DEF");

	/*  Surveyor gets the responses. */
	test_recv(surveyor, "DEF");
	test_recv(surveyor, "DEF");

	/*  There are no more responses. Surveyor hits the deadline. */
	rc = nn_recv(surveyor, buf, sizeof(buf), 0);
	errno_assert(rc == -1 && nn_errno() == ETIMEDOUT);

	/*  Third respondent answers (it have already missed the deadline). */
	test_recv(respondent3, "ABC");
	test_send(respondent3, "GHI");

	/*  Surveyor initiates new survey. */
	test_send(surveyor, "ABC");

	/*  Check that stale response from third respondent is not delivered.
	 */
	rc = nn_recv(surveyor, buf, sizeof(buf), 0);
	errno_assert(rc == -1 && nn_errno() == ETIMEDOUT);

	/* Check that subsequent attempt to recv with no survey pending is
	 * EFSM. */
	nn_sleep(1000); // nng - sleep a bit as there may be a thread race
	rc = nn_recv(surveyor, buf, sizeof(buf), 0);
	errno_assert(rc == -1 && nn_errno() == EFSM);

	test_close(surveyor);
	test_close(respondent1);
	test_close(respondent2);
	test_close(respondent3);

	return 0;
}
