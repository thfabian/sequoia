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
  tinyobjloader
  DOWNLOAD_DIR "${SEQUOIA_EXTERNAL_DOWNLOAD_DIR}"
  URL ${tinyobjloader_url}
  URL_MD5 ${tinyobjloader_md5}
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/tinyobjloader"
  INSTALL_DIR "${SEQUOIA_EXTERNAL_INSTALL_PREFIX}/tinyobjloader"
  CMAKE_CACHE_ARGS
    ${SEQUOIA_EXTERNAL_CMAKE_ARGS}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    -DTINYOBJLOADER_COMPILATION_SHARED:BOOL=${BUILD_SHARED_LIBS}
    -DTINYOBJLOADER_BUILD_TEST_LOADER:BOOL=OFF
    -DTINYOBJLOADER_USE_DOUBLE:BOOL=OFF
)

ExternalProject_Get_Property(tinyobjloader install_dir)
set(tinyobjloader_DIR "${install_dir}/lib/tinyobjloader/cmake" CACHE INTERNAL "")

sequoia_append_and_export_variable(
   SEQUOIA_EXTERNAL_CMAKE_ARGS 
  "-Dtinyobjloader_DIR:PATH=${tinyobjloader_DIR}" 
)