#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

macro (CheckSanitizer)

    if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
        set(NNG_SAN_LIST none address leak memory thread undefined)
    elseif (CMAKE_C_COMPILER_ID STREQUAL "Clang")
        set(NNG_SAN_LIST none address leak memory thread undefined)
    elseif (CMAKE_C_COMPILER_ID STREQUAL "AppleClang")
        set(NNG_SAN_LIST none address thread undefined) 
    else ()
        set(NNG_SAN_LIST none)
    endif ()
    set (NNG_SANITIZER none CACHE STRING "Sanitizer to use (clang or gcc).")
    set_property(CACHE NNG_SANITIZER PROPERTY STRINGS ${NNG_SAN_LIST})
    mark_as_advanced (NNG_SANITIZER)
    if (DEBUG AND ASAN)
        set (NNG_SANITIZER address)
	endif (DEBUG AND ASAN)

    if (NOT NNG_SANITIZER STREQUAL "none")
        set (NNG_C_FLAG_SANITIZER "-fsanitize=${NNG_SANITIZER}")
        message(STATUS "Enabling sanitizer: ${NNG_C_FLAG_SANITIZER}")
    endif()
endmacro ()
