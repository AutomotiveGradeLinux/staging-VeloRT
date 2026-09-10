#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
nng_directory(transport)

add_subdirectory(socket)
add_subdirectory(inproc)
add_subdirectory(ipc)
add_subdirectory(tcp)
add_subdirectory(tls)
add_subdirectory(ws)
add_subdirectory(mqtts)
add_subdirectory(zerotier)
add_subdirectory(mqtt)
add_subdirectory(mqttws)
