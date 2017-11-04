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

ExternalProject_Add(
  spdlog
  DOWNLOAD_DIR "${SEQUOIA_EXTERNAL_DOWNLOAD_DIR}/spdlog"
  URL ${spdlog_url}
  URL_MD5 ${spdlog_md5}
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/spdlog"
  INSTALL_DIR "${SEQUOIA_EXTERNAL_INSTALL_PREFIX}/spdlog"
  CMAKE_ARGS
    ${SEQUOIA_EXTERNAL_CMAKE_ARGS}
    ${SEQUOIA_EXTERNAL_PROJECTS_CMAKE_ARGS}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    -DSPDLOG_BUILD_EXAMPLES:BOOL=OFF
    -DSPDLOG_BUILD_TESTING:BOOL=OFF
)

ExternalProject_Get_Property(spdlog install_dir)
set(spdlog_DIR "${install_dir}/lib/cmake/spdlog" CACHE INTERNAL "")

sequoia_export_package(
  PACKAGE spdlog 
  CMAKE_ARGS 
    "-Dspdlog_DIR:PATH=${spdlog_DIR}"
)
