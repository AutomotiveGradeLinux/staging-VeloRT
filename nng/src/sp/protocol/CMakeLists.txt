#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
nng_directory(protocol)

add_subdirectory(bus0)
add_subdirectory(pair0)
add_subdirectory(pair1)
add_subdirectory(pipeline0)
add_subdirectory(pubsub0)
add_subdirectory(reqrep0)
add_subdirectory(survey0)

#NANOMQ MQTT library
add_subdirectory(mqtt)

