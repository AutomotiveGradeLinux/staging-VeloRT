#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

NNGCAT=${NNGCAT:=$1}
NNGCAT=${NNGCAT:-./nngcat}

echo -n "Verify nngcat help: "
if ${NNGCAT} --help >/dev/null 2>&1
then
	echo "Failed: help didn't return 1"
	exit 1
fi
x=$(${NNGCAT} --help 2>&1)
if [[ ${x} =~ "Usage:" ]]
then
	echo "pass"
	exit 0
fi

echo "Failed: usage did not match"
echo "Output:"
echo "$x"
exit 1
