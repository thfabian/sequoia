##===------------------------------------------------------------------------------*- CMake -*-===##
##                         _____                        _
##                        / ____|                      (_)
##                       | (___   ___  __ _ _   _  ___  _  __ _
##                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
##                        ____) |  __/ (_| | |_| | (_) | | (_| |
##                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine (2016-2017)
##                                       | |
##                                       |_|
##
## This file is distributed under the MIT License (MIT).
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

get_filename_component(current_dir ${CMAKE_CURRENT_LIST_FILE} PATH)

configure_file(
  "${current_dir}/templates/gtest.CMakeLists.txt.in"
  "${CMAKE_CURRENT_BINARY_DIR}/gtest.CMakeLists.txt"
  @ONLY
)

ExternalProject_Add(
  gtest
  DOWNLOAD_DIR "${SEQUOIA_EXTERNAL_DOWNLOAD_DIR}"
  DOWNLOAD_NAME "gtest-${gtest_version}.tar.gz"
  URL ${gtest_url}
  URL_MD5 ${gtest_md5}
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/gtest"
  INSTALL_DIR "${SEQUOIA_EXTERNAL_INSTALL_PREFIX}/gtest"
  PATCH_COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_BINARY_DIR}/gtest.CMakeLists.txt"
    "<SOURCE_DIR>/CMakeLists.txt"
  CMAKE_CACHE_ARGS
    ${SEQUOIA_EXTERNAL_CMAKE_ARGS}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    -Dgtest_force_shared_crt:BOOL=ON
)

ExternalProject_Get_Property(gtest install_dir)
set(GTEST_ROOT "${install_dir}" CACHE INTERNAL "")

sequoia_export_package(
  PACKAGE gtest 
  CMAKE_ARGS 
    "-DGTEST_ROOT:PATH=${GTEST_ROOT}" 
)
