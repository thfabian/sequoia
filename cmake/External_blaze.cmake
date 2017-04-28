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
  blaze
  DOWNLOAD_DIR ${download_dir}
  URL ${blaze_url}
  URL_MD5 ${blaze_md5}
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/blaze"
  INSTALL_DIR "${Sequoia_INSTALL_PREFIX}"
  PATCH_COMMAND ""
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ${CMAKE_COMMAND} -E make_directory <INSTALL_DIR>/include/blaze && 
                  ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/blaze/ <INSTALL_DIR>/include/blaze/
)

ExternalProject_Get_Property(blaze install_dir)
set(BLAZE_ROOT "${install_dir}" CACHE INTERNAL "")

list(APPEND Sequoia_THIRDPARTYLIBS_ARGS "-DBLAZE_ROOT:PATH=${BLAZE_ROOT}")
