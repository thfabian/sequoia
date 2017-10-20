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

include(ExternalProject)

ExternalProject_Add(
  glfw3
  DOWNLOAD_DIR "${SEQUOIA_EXTERNAL_DOWNLOAD_DIR}"
  URL ${glfw3_url}
  URL_MD5 ${glfw3_md5}
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/glfw3"
  INSTALL_DIR "${SEQUOIA_EXTERNAL_INSTALL_PREFIX}/glfw3"
  CMAKE_CACHE_ARGS
    ${SEQUOIA_EXTERNAL_CMAKE_ARGS}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    -DGLFW_BUILD_DOCS:BOOL=OFF
    -DGLFW_BUILD_EXAMPLES:BOOL=OFF
    -DGLFW_BUILD_TESTS:BOOL=OFF
)

ExternalProject_Get_Property(glfw3 install_dir)
set(glfw3_DIR "${install_dir}/lib/cmake/glfw3" CACHE INTERNAL "")

set(SEQUOIA_EXTERNAL_CMAKE_ARGS 
  "${SEQUOIA_EXTERNAL_CMAKE_ARGS}" 
  "-Dglfw3_DIR:PATH=${glfw3_DIR}"
  PARENT_SCOPE
)
