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
  gli
  DOWNLOAD_DIR ${download_dir}
  URL ${gli_url}
  URL_MD5 ${gli_md5}
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/gli"
  INSTALL_DIR "${Sequoia_INSTALL_PREFIX}/gli"
  PATCH_COMMAND ""
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ${CMAKE_COMMAND} -E make_directory <INSTALL_DIR>/gli && 
                  ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/gli/ <INSTALL_DIR>/gli/
)

ExternalProject_Get_Property(gli install_dir)
set(GLI_ROOT "${install_dir}" CACHE INTERNAL "")

list(APPEND Sequoia_THIRDPARTYLIBS_ARGS "-DGLI_ROOT:PATH=${GLI_ROOT}")