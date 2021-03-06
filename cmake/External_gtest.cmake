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

ExternalProject_Add(
  gtest
  DOWNLOAD_DIR "${SEQUOIA_EXTERNAL_DOWNLOAD_DIR}/gtest"
  DOWNLOAD_NAME "gtest-${gtest_version}.tar.gz"
  URL ${gtest_url}
  URL_MD5 ${gtest_md5}
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/gtest"
  INSTALL_DIR "${SEQUOIA_EXTERNAL_INSTALL_PREFIX}/gtest"
  PATCH_COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${current_dir}/templates/gtest.internal_utils.cmake"
    "<SOURCE_DIR>/googletest/cmake/internal_utils.cmake"
  CMAKE_CACHE_ARGS
    ${SEQUOIA_EXTERNAL_CMAKE_ARGS}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    -DBUILD_GMOCK:BOOL=OFF
    -DBUILD_GTEST:BOOL=ON
    -Dgtest_force_shared_crt:BOOL=ON
)

ExternalProject_Get_Property(gtest install_dir)
set(GTEST_ROOT "${install_dir}" CACHE INTERNAL "")

sequoia_export_package(
  PACKAGE gtest 
  CMAKE_ARGS 
    "-DGTEST_ROOT:PATH=${GTEST_ROOT}" 
)
