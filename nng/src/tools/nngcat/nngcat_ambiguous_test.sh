#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

NNGCAT=${NNGCAT:=$1}
NNGCAT=${NNGCAT:-./nngcat}
CMD="${NNGCAT} --re --dial=tcp://127.0.0.1:27272"

echo -n "Verify ambiguous options fail: "
if ${CMD} >/dev/null 2>&1
then
	echo "Failed: ambigous accepted"
	exit 1
fi
x=$(${CMD} 2>&1)
if [[ ${x} =~ "ambiguous" ]]
then
	echo "pass"
	exit 0
fi

echo "Failed: error did not match"
exit 1
