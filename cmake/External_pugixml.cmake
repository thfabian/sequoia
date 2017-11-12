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
  pugixml
  DOWNLOAD_DIR "${SEQUOIA_EXTERNAL_DOWNLOAD_DIR}/pugixml"
  URL ${pugixml_url}
  URL_MD5 ${pugixml_md5}
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/pugixml"
  INSTALL_DIR "${SEQUOIA_EXTERNAL_INSTALL_PREFIX}/pugixml"
  CMAKE_ARGS
    ${SEQUOIA_EXTERNAL_CMAKE_ARGS}
    ${SEQUOIA_EXTERNAL_PROJECTS_CMAKE_ARGS}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    -DBUILD_TESTS=OFF
)

ExternalProject_Get_Property(pugixml install_dir)
set(pugixml_DIR "${install_dir}/lib/cmake/pugixml" CACHE INTERNAL "")

sequoia_export_package(
  PACKAGE pugixml 
  CMAKE_ARGS 
    "-Dpugixml_DIR:PATH=${pugixml_DIR}"
)
