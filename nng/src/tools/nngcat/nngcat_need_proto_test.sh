#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

NNGCAT=${NNGCAT:=$1}
NNGCAT=${NNGCAT:-./nngcat}

echo -n "Verify a protocol is needed: "
if ${NNGCAT} --dial=tcp://127.0.0.1:8989 >/dev/null 2>&1
then
	echo "Failed: protocol should be required"
	exit 1
fi
echo "pass"
exit 0
