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
  DOWNLOAD_DIR ${download_dir}
  URL ${tinyobjloader_url}
  URL_MD5 ${tinyobjloader_md5}
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/tinyobjloader"
  INSTALL_DIR "${Sequoia_INSTALL_PREFIX}/tinyobjloader"
  CMAKE_CACHE_ARGS
    ${Sequoia_THIRDPARTY_CMAKE_ARGS}
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DCMAKE_CONFIGURATION_TYPES:STRING=${CMAKE_CONFIGURATION_TYPES}
    -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    -DTINYOBJLOADER_COMPILATION_SHARED:BOOL=${BUILD_SHARED_LIBS}
    -DTINYOBJLOADER_BUILD_TEST_LOADER:BOOL=OFF
    -DTINYOBJLOADER_USE_DOUBLE:BOOL=OFF
)

ExternalProject_Get_Property(tinyobjloader install_dir)
set(tinyobjloader_DIR "${install_dir}/lib/tinyobjloader/cmake" CACHE INTERNAL "")

list(APPEND Sequoia_THIRDPARTY_CMAKE_ARGS "-Dtinyobjloader_DIR:PATH=${tinyobjloader_DIR}")
