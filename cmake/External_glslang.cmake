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
  glslang
  DOWNLOAD_DIR ${download_dir}
  URL ${glslang_url}
  URL_MD5 ${glslang_md5}
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/glslang"
  INSTALL_DIR "${Sequoia_INSTALL_PREFIX}/glslang"
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    -DBUILD_TESTING:BOOL=OFF
)

ExternalProject_Get_Property(glslang install_dir)
set(GLSLANG_ROOT "${install_dir}" CACHE INTERNAL "")

list(APPEND Sequoia_THIRDPARTYLIBS_ARGS "-DGLSLANG_ROOT:PATH=${GLSLANG_ROOT}")

