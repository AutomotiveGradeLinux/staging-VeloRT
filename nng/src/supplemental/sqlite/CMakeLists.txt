#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
if (NNG_ENABLE_SQLITE)
  nng_sources(sqlite3.c)

  if (NNG_PLATFORM_POSIX)
    nng_link_libraries(dl)
  elseif (NNG_PLATFORM_WINDOWS)
    nng_link_libraries(ws2_32)
  endif()

  nng_test(sqlite3_test)
endif (NNG_ENABLE_SQLITE)


