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
  glfw3
  DOWNLOAD_DIR ${download_dir}
  URL ${glfw3_url}
  URL_MD5 ${glfw3_md5}
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/glfw3"
  INSTALL_DIR "${Sequoia_INSTALL_PREFIX}/glfw3"
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DCMAKE_CONFIGURATION_TYPES:STRING=${CMAKE_CONFIGURATION_TYPES}
    -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    -DGLFW_BUILD_DOCS:BOOL=OFF
    -DGLFW_BUILD_EXAMPLES:BOOL=OFF
    -DGLFW_BUILD_TESTS:BOOL=OFF
)

ExternalProject_Get_Property(glfw3 install_dir)
set(glfw3_DIR "${install_dir}/lib/cmake/glfw3" CACHE INTERNAL "")

list(APPEND Sequoia_THIRDPARTY_CMAKE_ARGS "-Dglfw3_DIR:PATH=${glfw3_DIR}")


