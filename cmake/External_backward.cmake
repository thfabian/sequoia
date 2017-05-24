##===------------------------------------------------------------------------------*- CMake -*-===##
##                         _____                        _       
##                        / ____|                      (_)      
##                       | (___   ___  __ _ _   _  ___  _  __ _ 
##                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
##                        ____) |  __/ (_| | |_| | (_) | | (_| |
##                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine
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
  PATCH_COMMAND ""
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ${CMAKE_COMMAND} -E make_directory <INSTALL_DIR>/backward && 
                  ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/backward.hpp <INSTALL_DIR>/backward/backward.hpp
)

ExternalProject_Get_Property(backward install_dir)
set(BACKWARD_ROOT "${install_dir}" CACHE INTERNAL "")

list(APPEND Sequoia_THIRDPARTYLIBS_ARGS "-DBACKWARD_ROOT:PATH=${BACKWARD_ROOT}")

