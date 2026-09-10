#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

set(_WOLFSSL_ROOT_HINTS ${WOLFSSL_ROOT_DIR} ENV WOLFSSL_ROOT_DIR)

include(FindPackageHandleStandardArgs)
include(CMakePushCheckState)

find_path(WOLFSSL_INCLUDE_DIR
    NAMES wolfssl/ssl.h
    HINTS ${_WOLFSSL_ROOT_HINTS}
    PATHS /usr/local
    PATH_SUFFIXES include)

find_library(WOLFSSL_LIBRARY
    NAMES wolfssl
    HINTS ${_WOLFSSL_ROOT_HINTS}
    PATHS /usr/local
    PATH_SUFFIXES lib)

set(WOLFSSL_LIBRARIES ${WOLFSSL_LIBRARY})
if (${WOLFSSL_LIBRARY-NOTFOUND})
    message(FATAL_ERROR "Failed to find wolfSSL library")
endif()

mark_as_advanced(WOLFSSL_INCLUDE_DIR WOLFSSL_LIBRARY)

# Extract the version from the header... hopefully it matches the library.
file(STRINGS ${WOLFSSL_INCLUDE_DIR}/wolfssl/version.h _WOLFSSL_VERLINE
    REGEX "^#define[ \t]+LIBWOLFSSL_VERSION_STRING[\t ].*")
string(REGEX REPLACE ".*WOLFSSL_VERSION_STRING[\t ]+\"(.*)\"" "\\1" WOLFSSL_VERSION ${_WOLFSSL_VERLINE})

add_library(wolfSSL::wolfssl UNKNOWN IMPORTED)

set_target_properties(wolfSSL::wolfssl PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${WOLFSSL_INCLUDE_DIR}")
set_target_properties(wolfSSL::wolfssl PROPERTIES IMPORTED_LOCATION "${WOLFSSL_LIBRARY}")

set(wolfSSL_TARGET wolfssl::wolfssl)

find_package_handle_standard_args(wolfSSL
    REQUIRED_VARS WOLFSSL_LIBRARY WOLFSSL_INCLUDE_DIR VERSION_VAR WOLFSSL_VERSION)
