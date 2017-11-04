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
  zlib
  DOWNLOAD_DIR "${SEQUOIA_EXTERNAL_DOWNLOAD_DIR}/zlib"
  URL ${zlib_url}
  URL_MD5 ${zlib_md5}
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/zlib"
  INSTALL_DIR "${SEQUOIA_EXTERNAL_INSTALL_PREFIX}/zlib"
  CMAKE_CACHE_ARGS
    ${SEQUOIA_EXTERNAL_CMAKE_ARGS}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
)

ExternalProject_Get_Property(zlib install_dir)
set(ZLIB_ROOT "${install_dir}" CACHE INTERNAL "")

sequoia_export_package(
  PACKAGE zlib 
  CMAKE_ARGS "-DZLIB_ROOT:PATH=${ZLIB_ROOT}"
)
