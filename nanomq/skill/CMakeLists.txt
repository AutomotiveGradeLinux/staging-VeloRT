#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.13)
project(nanomq_skill C)

set(CMAKE_C_STANDARD 11)

add_library(nanomq_skill STATIC
    src/nano_skill_time.c
    src/nano_skill_window.c
    src/nano_skill_batch.c
    src/nano_skill_dbc_stub.c
)

target_include_directories(nanomq_skill PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

find_package(Threads REQUIRED)
target_link_libraries(nanomq_skill PUBLIC Threads::Threads)

