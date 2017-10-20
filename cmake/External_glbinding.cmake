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
  glbinding
  DOWNLOAD_DIR "${SEQUOIA_EXTERNAL_DOWNLOAD_DIR}"
  URL ${glbinding_url}
  URL_MD5 ${glbinding_md5}
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/glbinding"
  INSTALL_DIR "${SEQUOIA_EXTERNAL_INSTALL_PREFIX}/glbinding"
  CMAKE_CACHE_ARGS
    ${SEQUOIA_EXTERNAL_CMAKE_ARGS}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    -DOPTION_BUILD_DOCS:BOOL=OFF
    -DOPTION_BUILD_EXAMPLES:BOOL=OFF
    -DOPTION_BUILD_TESTS:BOOL=OFF
)

ExternalProject_Get_Property(glbinding install_dir)
set(GLBINDING_ROOT "${install_dir}" CACHE INTERNAL "")

set(SEQUOIA_EXTERNAL_CMAKE_ARGS 
  "${SEQUOIA_EXTERNAL_CMAKE_ARGS}" 
  "-DGLBINDING_ROOT:PATH=${GLBINDING_ROOT}"
  PARENT_SCOPE
)
