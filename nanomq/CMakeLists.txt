#
# Copyright 2025 NanoMQ Team, Inc. <jaylin@emqx.io>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 2.8.12...3.10)

project(nanomq)
SET(CMAKE_C_STANDARD 99)

if(DEBUG AND ASAN)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fsanitize=address")
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=address")
endif(DEBUG AND ASAN)

if(DEBUG AND TSAN)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fsanitize=thread")
  # SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=thread")
endif(DEBUG AND TSAN)

aux_source_directory(. DIRSRCS)

include_directories(./)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/../nng/include/nng)
if(ENABLE_JWT)
  include_directories(${PROJECT_SOURCE_DIR}/../extern/l8w8jwt/include)
endif(ENABLE_JWT)

add_subdirectory(apps)
add_subdirectory(tests)
#add_subdirectory(plugin)

# set(PARALLEL 128 CACHE STRING "Parallelism (min 4, max 1000)")

set(SOURCES
    process.c
    bridge.c
    pub_handler.c
    sub_handler.c
    unsub_handler.c
    hashmap.c
    rest_api.c
    web_server.c
    webhook_inproc.c
    webhook_post.c
    aws_bridge.c
    nanomq_rule.c
    conf_api.c
    cmd_proc.c
    acl_handler.c
    acl_hazard.c
    mqtt_api.c
    stream_plugin_runtime.c
    stream_inject.c
    nanomq.c
    plugin_spi_stream.c
    plugin_canp_stream.c
    apps/broker.c
    apps/nng_bridge.c
)

if(NNG_ENABLE_PLUGIN)
  set(SOURCES ${SOURCES} plugin/plugin.c)
endif(NNG_ENABLE_PLUGIN)

if(NNG_ENABLE_PARQUET)
  include_directories(${CMAKE_SOURCE_DIR}/nanomq/extern/include)
  set(SOURCES ${SOURCES} extern/aes_gcm.c)
endif()

if(ENABLE_FILETRANSFER AND NNG_ENABLE_PARQUET)
  ADD_DEFINITIONS(-DENABLE_FILETRANSFER)
  include_directories(${CMAKE_SOURCE_DIR}/nanomq/extern/include)
  set(SOURCES ${SOURCES} extern/file_transfer_nano.c)
endif(ENABLE_FILETRANSFER AND NNG_ENABLE_PARQUET)

include_directories(${FOUNDATION_INCLUDE_DIR})

if(BUILD_STATIC_LIB)
  add_library(nanomq  ${SOURCES})
  # Unit tests (NANOMQ_TESTS) only link nanomq; PUBLIC keeps nng and optional
  # deps on the test link line. Static nanomq is not install(EXPORT)'d.
  set(NANOMQ_DEPS_SCOPE PUBLIC)
elseif (BUILD_SHARED_LIBS) 
  add_library(nanomq SHARED ${SOURCES})
  set_property(TARGET nanomq PROPERTY POSITION_INDEPENDENT_CODE 1)
  set(NANOMQ_DEPS_SCOPE PRIVATE)
else()
  add_executable(nanomq ${SOURCES})
  # Export broker symbols so dlopen()'d plugins can resolve nano_* APIs
  # implemented inside the broker (A2 minimal shell path).
  set_target_properties(nanomq PROPERTIES ENABLE_EXPORTS ON)
  # PRIVATE so install(EXPORT nanomqConfig) does not require JWT/QUIC targets
  # that are not in any export set.
  set(NANOMQ_DEPS_SCOPE PRIVATE)
endif()

if(ENABLE_MYSQL)

  include(FindPkgConfig)
  pkg_check_modules(LIBMYSQLCLIENT REQUIRED mysqlclient)

  foreach(FLAG ${LIBMYSQLCLIENT_CFLAGS_OTHER})
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FLAG}")
  endforeach()

  link_directories(${LIBMYSQLCLIENT_LIBRARY_DIRS})
  include_directories(${MYSQL_INCLUDE_DIR})
  include_directories(${LIBMYSQLCLIENT_INCLUDE_DIRS})
  # message(STATUS, "${LIBMYSQLCLIENT_INCLUDE_DIRS}")
  target_link_libraries(nanomq ${NANOMQ_DEPS_SCOPE} ${LIBMYSQLCLIENT_LIBRARIES})

endif(ENABLE_MYSQL)

if(ENABLE_POSTGRESQL)

  find_package(PostgreSQL REQUIRED)

  # Link to the PostgreSQL C library
  target_link_libraries(nanomq ${NANOMQ_DEPS_SCOPE} ${PostgreSQL_LIBRARIES})

  # Include the PostgreSQL C library headers
  target_include_directories(nanomq PUBLIC ${PostgreSQL_INCLUDE_DIRS})

endif(ENABLE_POSTGRESQL)

if(ENABLE_TIMESCALEDB)

  find_package(PostgreSQL REQUIRED)

  # Link to the PostgreSQL C library
  target_link_libraries(nanomq ${NANOMQ_DEPS_SCOPE} ${PostgreSQL_LIBRARIES})

  # Include the PostgreSQL C library headers
  target_include_directories(nanomq PUBLIC ${PostgreSQL_INCLUDE_DIRS})

endif(ENABLE_TIMESCALEDB)

if(ENABLE_AWS_BRIDGE)
  target_link_libraries(nanomq ${NANOMQ_DEPS_SCOPE}
      aws_iot_mqtt
      aws_iot_backoff_algorithm
      sockets_posix
      openssl_posix
      clock_posix)

endif(ENABLE_AWS_BRIDGE)

if(ENABLE_FILETRANSFER)
  find_package(OpenSSL REQUIRED) #TODO
  target_link_libraries(nanomq ${NANOMQ_DEPS_SCOPE} OpenSSL::Crypto)
endif(ENABLE_FILETRANSFER)

if(NNG_ENABLE_PARQUET)
  if(OPENSSL_ROOT_DIR)
      set(OPENSSL_INCLUDE_DIR ${OPENSSL_ROOT_DIR}/include)
      set(OPENSSL_LIB_DIR ${OPENSSL_ROOT_DIR}/lib)
      if (NANO_PLATFORM_POSIX)
        nng_link_libraries(
          ${OPENSSL_LIB_DIR}/libssl.a
          ${OPENSSL_LIB_DIR}/libcrypto.a
        )
      else()
        nng_link_libraries(
          ${OPENSSL_LIB_DIR}/VC/x64/MT/libssl_static.lib
          ${OPENSSL_LIB_DIR}/VC/x64/MT/libcrypto_static.lib
        )
      endif(NANO_PLATFORM_POSIX)
      target_include_directories(nanomq PUBLIC ${OPENSSL_INCLUDE_DIR})
  else()
      find_package(OpenSSL REQUIRED)
      nng_link_libraries(OpenSSL::SSL OpenSSL::Crypto)
  endif(OPENSSL_ROOT_DIR)
endif(NNG_ENABLE_PARQUET)

if(ENABLE_JWT)
  target_link_libraries(nanomq ${NANOMQ_DEPS_SCOPE} l8w8jwt)
endif(ENABLE_JWT)

if(NNG_ENABLE_QUIC)
  target_link_libraries(nanomq ${NANOMQ_DEPS_SCOPE} msquic OpenSSLQuic)
endif(NNG_ENABLE_QUIC)

if(BUILD_QUIC_CLI)
  target_link_libraries(nanomq ${NANOMQ_DEPS_SCOPE} nng msquic)
endif(BUILD_QUIC_CLI)

if (CMAKE_INSTALL_RPATH)
  set_target_properties(nanomq PROPERTIES LINK_FLAGS "-Wl,-rpath,${CMAKE_INSTALL_RPATH}")
  message("Dynamic link rpath: ${CMAKE_INSTALL_RPATH}")
endif()

if (NOT WIN32)
  if (NNG_ENABLE_HTTP)
    target_link_libraries(nanomq ${NANOMQ_DEPS_SCOPE} nng m)
  endif()
else ()
  nng_link_libraries(psapi)
endif()

if(BUILD_STATIC)
  target_link_libraries(nanomq ${NANOMQ_DEPS_SCOPE} nng -static)
else()
    target_link_libraries(nanomq ${NANOMQ_DEPS_SCOPE} nng)
endif()

target_compile_definitions(nanomq PRIVATE -DPARALLEL=${PARALLEL})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

function(nanomq_resolve_static_archive out_var target_name)
  set(resolved "")
  if(target_name AND EXISTS "${target_name}" AND
      target_name MATCHES "\\.(a|lib)$")
    set(resolved ${target_name})
  elseif(TARGET ${target_name})
    foreach(prop
        IMPORTED_LOCATION
        IMPORTED_LOCATION_NOCONFIG
        IMPORTED_LOCATION_RELEASE
        IMPORTED_LOCATION_RELWITHDEBINFO
        IMPORTED_LOCATION_MINSIZEREL
        IMPORTED_LOCATION_DEBUG)
      get_target_property(candidate ${target_name} ${prop})
      if(candidate AND EXISTS "${candidate}" AND candidate MATCHES "\\.(a|lib)$")
        set(resolved ${candidate})
        break()
      endif()
    endforeach()
  endif()
  set(${out_var} "${resolved}" PARENT_SCOPE)
endfunction()

function(nanomq_append_static_archive list_var archive_label)
  set(resolved "")
  foreach(target_name ${ARGN})
    nanomq_resolve_static_archive(candidate ${target_name})
    if(candidate)
      set(resolved ${candidate})
      break()
    endif()
  endforeach()

  if(resolved AND EXISTS "${resolved}")
    set(updated_archives ${${list_var}})
    list(APPEND updated_archives ${resolved})
    set(${list_var} ${updated_archives} PARENT_SCOPE)
  else()
    message(WARNING
        "BUILD_STATIC_LIB could not resolve a static ${archive_label} archive; raw consumers like cgo must link it explicitly.")
  endif()
endfunction()

# Package all static files into one libnanomq.a
if(BUILD_STATIC_LIB)
  set(FULL_OUTPUT_PATH ${CMAKE_BINARY_DIR}/libnanomq.a)
  set (tmp ${CMAKE_BINARY_DIR}/tmp)
  set(NANOMQ_MERGED_ARCHIVES
      ${CMAKE_BINARY_DIR}/nanomq/libnanomq.a
      ${CMAKE_BINARY_DIR}/nng/libnng.a)

  # Raw static consumers such as cgo cannot see CMake's transitive
  # metadata, so merge libcrypto when AES/OpenSSL helpers are compiled in.
  if(NNG_ENABLE_PARQUET OR ENABLE_FILETRANSFER)
    set(NANOMQ_OPENSSL_CRYPTO_ARCHIVE "")
    if(OPENSSL_ROOT_DIR AND NOT OPENSSL_LIB_DIR)
      set(OPENSSL_LIB_DIR ${OPENSSL_ROOT_DIR}/lib)
    endif()

    if(OPENSSL_LIB_DIR)
      if(NANO_PLATFORM_POSIX)
        set(NANOMQ_OPENSSL_CRYPTO_ARCHIVE ${OPENSSL_LIB_DIR}/libcrypto.a)
      else()
        set(NANOMQ_OPENSSL_CRYPTO_ARCHIVE
            ${OPENSSL_LIB_DIR}/VC/x64/MT/libcrypto_static.lib)
      endif()
    else()
      nanomq_resolve_static_archive(
          NANOMQ_OPENSSL_CRYPTO_ARCHIVE OpenSSL::Crypto)
    endif()

    if(NANOMQ_OPENSSL_CRYPTO_ARCHIVE AND
        EXISTS "${NANOMQ_OPENSSL_CRYPTO_ARCHIVE}")
      list(APPEND NANOMQ_MERGED_ARCHIVES ${NANOMQ_OPENSSL_CRYPTO_ARCHIVE})
    else()
      message(WARNING
          "BUILD_STATIC_LIB could not resolve a static libcrypto archive; raw consumers like cgo must link libcrypto explicitly.")
    endif()
  endif()

  if(NNG_ENABLE_TLS)
    # TLS transport pulls these in from nng, but raw archive consumers do not
    # see CMake target metadata and need them folded into the final libnanomq.a.
    nanomq_append_static_archive(
      NANOMQ_MERGED_ARCHIVES mbedtls ${_MBEDTLS_LIBRARY} mbedtls MbedTLS::mbedtls)
    nanomq_append_static_archive(
      NANOMQ_MERGED_ARCHIVES mbedx509 ${_MBEDX509_LIBRARY} mbedx509 MbedTLS::mbedx509)
    nanomq_append_static_archive(
      NANOMQ_MERGED_ARCHIVES mbedcrypto ${_MBEDCRYPTO_LIBRARY} mbedcrypto MbedTLS::mbedcrypto)
  endif()

  list(REMOVE_DUPLICATES NANOMQ_MERGED_ARCHIVES)
  file(WRITE ${tmp} "create ${FULL_OUTPUT_PATH}\n")
  foreach(archive ${NANOMQ_MERGED_ARCHIVES})
    file(APPEND ${tmp} "addlib ${archive}\n")
  endforeach()
  file(APPEND ${tmp} "save\n")
  file(APPEND ${tmp} "end\n")

  add_custom_target(libnano
          COMMAND ar -M < ${CMAKE_BINARY_DIR}/tmp
          COMMAND rm ${CMAKE_BINARY_DIR}/tmp
          COMMAND rm ${CMAKE_BINARY_DIR}/nanomq/libnanomq.a
          WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
          DEPENDS nanomq nng
          )
endif(BUILD_STATIC_LIB)

# TODO INSTALL

if(BUILD_STATIC_LIB OR BUILD_SHARED_LIBS)
  # install(
  #   FILES ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libnanomq.a
  #   LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR})

else()
  install(
    TARGETS nanomq
    EXPORT nanomqConfig
    # ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    # LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT services)

  install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    COMPONENT services)

  install(
    EXPORT nanomqConfig
    DESTINATION share/nanomq/cmake
    COMPONENT services)

endif()

if (NANOMQ_TESTS)
    add_definitions(-DENABLE_NANOMQ_TESTS)
endif(NANOMQ_TESTS)