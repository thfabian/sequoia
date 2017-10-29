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
  backward
  DOWNLOAD_DIR "${SEQUOIA_EXTERNAL_DOWNLOAD_DIR}"
  URL ${backward_url}
  URL_MD5 ${backward_md5}
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/backward"
  INSTALL_DIR "${SEQUOIA_EXTERNAL_INSTALL_PREFIX}/backward"
  CMAKE_ARGS
    ${SEQUOIA_EXTERNAL_CMAKE_ARGS}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    -DBACKWARD_SHARED:BOOL=${BUILD_SHARED_LIBS}
    -DBACKWARD_TESTS:BOOL=OFF
    -DSTACK_DETAILS_AUTO_DETECT:BOOL=ON
)

ExternalProject_Get_Property(backward install_dir)

sequoia_export_package(
  PACKAGE backward 
  CMAKE_ARGS 
    "-DBackward_DIR:PATH=${install_dir}/lib/backward"
    "-DBACKWARD_INCLUDE_DIR:PATH=${install_dir}/include"
)
