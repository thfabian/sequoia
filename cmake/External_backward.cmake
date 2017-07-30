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
  DOWNLOAD_DIR ${download_dir}
  URL ${backward_url}
  URL_MD5 ${backward_md5}
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/backward"
  INSTALL_DIR "${Sequoia_INSTALL_PREFIX}/backward"
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DCMAKE_CONFIGURATION_TYPES:STRING=${CMAKE_CONFIGURATION_TYPES}
    -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    -DBACKWARD_SHARED:BOOL=${BUILD_SHARED_LIBS}
    -DBACKWARD_TESTS:BOOL=OFF
    -DSTACK_DETAILS_AUTO_DETECT:BOOL=ON
)

ExternalProject_Get_Property(backward install_dir)

list(APPEND Sequoia_THIRDPARTY_CMAKE_ARGS "-DBackward_DIR:PATH=${install_dir}/lib/backward")
list(APPEND Sequoia_THIRDPARTY_PREFIX_PATH "${install_dir}")
