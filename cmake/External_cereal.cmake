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
  cereal
  DOWNLOAD_DIR "${SEQUOIA_EXTERNAL_DOWNLOAD_DIR}"
  URL ${cereal_url}
  URL_MD5 ${cereal_md5}
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/cereal"
  INSTALL_DIR "${SEQUOIA_EXTERNAL_INSTALL_PREFIX}/cereal"
  PATCH_COMMAND ""
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ${CMAKE_COMMAND} -E make_directory <INSTALL_DIR>/include && 
                  ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/include <INSTALL_DIR>/include
)

ExternalProject_Get_Property(cereal install_dir)
set(CEREAL_ROOT "${install_dir}" CACHE INTERNAL "")

sequoia_export_package(
  PACKAGE cereal 
  CMAKE_ARGS 
    "-DCEREAL_ROOT:PATH=${CEREAL_ROOT}"
)
