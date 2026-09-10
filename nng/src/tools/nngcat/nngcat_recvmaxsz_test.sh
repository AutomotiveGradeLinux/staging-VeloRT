#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

NNGCAT=${NNGCAT:=$1}
NNGCAT=${NNGCAT:-./nngcat}
ADDR="ipc:///tmp/nngcat_recvmaxsz_test"
OUTPUT=/tmp/nngcat_recvmaxsz_test.$$.out

echo -n "Verify maximum receive size: "

trap "rm $OUTPUT" 0

${NNGCAT} --listen ${ADDR} --count=3 --recv-maxsz=5 --pull0 --quoted > $OUTPUT 2>/dev/null &
sleep 1
# for speed of execution, run these in the background, they should be ignored
${NNGCAT} --connect ${ADDR} --push0 --data "one"
${NNGCAT} --connect ${ADDR} --push0 --data "55555"
${NNGCAT} --connect ${ADDR} --push0 --data "666666"
${NNGCAT} --connect ${ADDR} --push0 --data "7777777"
${NNGCAT} --connect ${ADDR} --push0 --data "88888"

wait $bgid 2>/dev/null

sum=$(cksum ${OUTPUT})
sum=${sum%% *}

# This matches 3 lines of "one", "55555", "88888".
if [[ ${sum} == 4122906158 ]]
then
	echo "pass"
	exit 0
fi
echo "FAIL: Checksum failed (Wanted 3929078614 got ${sum})"
echo "OUTPUT:"
cat ${OUTPUT}

exit 1
