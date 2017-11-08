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
  assimp
  DOWNLOAD_DIR "${SEQUOIA_EXTERNAL_DOWNLOAD_DIR}/assimp"
  URL ${assimp_url}
  URL_MD5 ${assimp_md5}
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/assimp"
  INSTALL_DIR "${SEQUOIA_EXTERNAL_INSTALL_PREFIX}/assimp"
  CMAKE_ARGS
    ${SEQUOIA_EXTERNAL_CMAKE_ARGS}
    ${SEQUOIA_EXTERNAL_PROJECTS_CMAKE_ARGS}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    -DASSIMP_BUILD_TESTS:BOOL=OFF
    -DASSIMP_BUILD_SAMPLES:BOOL=OFF
    -DASSIMP_BUILD_ASSIMP_TOOLS:BOOL=OFF

    -DASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT:BOOL=OFF
    -DASSIMP_BUILD_OBJ_IMPORTER:BOOL=ON
)

ExternalProject_Get_Property(assimp install_dir)
set(assimp_DIR "${install_dir}/lib/cmake/assimp-4.0" CACHE INTERNAL "")

sequoia_export_package(
  PACKAGE assimp 
  CMAKE_ARGS 
    "-Dassimp_DIR:PATH=${assimp_DIR}"
)
