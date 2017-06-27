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
  freeimage
  DOWNLOAD_DIR ${download_dir}
  URL ${freeimage_url}
  URL_MD5 ${freeimage_md5}
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/FreeImage"
  INSTALL_DIR "${Sequoia_INSTALL_PREFIX}/FreeImage"
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    -DFREEIMAGE_TESTING=OFF
)

ExternalProject_Get_Property(freeimage install_dir)
set(FREEIMAGE_ROOT "${install_dir}" CACHE INTERNAL "")

list(APPEND Sequoia_THIRDPARTYLIBS_ARGS "-DFREEIMAGE_ROOT:PATH=${FREEIMAGE_ROOT}")

