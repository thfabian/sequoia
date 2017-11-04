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
  fmt
  DOWNLOAD_DIR "${SEQUOIA_EXTERNAL_DOWNLOAD_DIR}/fmt"
  URL ${fmt_url}
  URL_MD5 ${fmt_md5}
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/fmt"
  INSTALL_DIR "${SEQUOIA_EXTERNAL_INSTALL_PREFIX}/fmt"
  CMAKE_ARGS
    ${SEQUOIA_EXTERNAL_CMAKE_ARGS}
    ${SEQUOIA_EXTERNAL_PROJECTS_CMAKE_ARGS}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    -DFMT_TEST:BOOL=OFF
    -DFMT_USE_CPP11:BOOL=ON
)

ExternalProject_Get_Property(fmt install_dir)
set(fmt_DIR "${install_dir}/lib/cmake/fmt" CACHE INTERNAL "")

sequoia_export_package(
  PACKAGE fmt 
  CMAKE_ARGS 
    "-Dfmt_DIR:PATH=${fmt_DIR}"
)
