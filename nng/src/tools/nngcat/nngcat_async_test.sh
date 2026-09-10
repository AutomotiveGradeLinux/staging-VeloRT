#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

NNGCAT=${NNGCAT:=$1}
NNGCAT=${NNGCAT:-./nngcat}
ADDR="ipc:///tmp/nngcat_async_test"

echo -n "Verify async connect: "

${NNGCAT} --async -d 1 --connect ${ADDR} --req0 -D "ping" &


answer=$( ${NNGCAT} --rep0 --recv-timeout=3 --listen ${ADDR} -D "pong" --ascii 2>/dev/null )

if [[ ${answer} == "ping" ]]
then
	echo "pass"
	exit 0
fi

echo "Failed: req did not match"
echo "RES: $answer"
exit 1
