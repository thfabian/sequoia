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
  glm
  DOWNLOAD_DIR "${SEQUOIA_EXTERNAL_DOWNLOAD_DIR}/glm"
  URL ${glm_url}
  URL_MD5 ${glm_md5}
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/glm"
  INSTALL_DIR "${SEQUOIA_EXTERNAL_INSTALL_PREFIX}/glm"
  PATCH_COMMAND ""
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ${CMAKE_COMMAND} -E make_directory <INSTALL_DIR>/glm && 
                  ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/glm/ <INSTALL_DIR>/glm/
)

ExternalProject_Get_Property(glm install_dir)
set(GLM_ROOT "${install_dir}" CACHE INTERNAL "")

sequoia_export_package(
  PACKAGE glm 
  CMAKE_ARGS 
    "-DGLM_ROOT:PATH=${GLM_ROOT}"
)
