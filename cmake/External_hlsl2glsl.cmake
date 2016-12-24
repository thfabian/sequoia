##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                      S E Q U O I A
##
## This file is distributed under the MIT License (MIT). 
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

ExternalProject_Add(
  hlsl2glsl
  DOWNLOAD_DIR ${download_dir}
  DOWNLOAD_NAME "hlsl2glsl-v${hlsl2glsl_version}.tar.gz"
  UPDATE_COMMAND ${update_command}
  URL ${hlsl2glsl_url}
  URL_MD5 ${hlsl2glsl_md5}
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/hlsl2glsl"
  INSTALL_DIR "${Sequoia_INSTALL_PREFIX}"
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
)

ExternalProject_Get_Property(hlsl2glsl install_dir)
set(HLSL2GLSL_HOME "${install_dir}" CACHE INTERNAL "")