##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                      S E Q U O I A
##
## This file is distributed under the MIT License (MIT). 
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

ExternalProject_Add(
  glsl-optimizer
  DOWNLOAD_DIR ${download_dir}
  DOWNLOAD_NAME "glsl-optimizer-v${glsl_optimizer_version}.tar.gz"
  UPDATE_COMMAND ${update_command}
  URL ${glsl_optimizer_url}
  URL_MD5 ${glsl_optimizer_md5}
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/glsl-optimizer"
  INSTALL_DIR "${Sequoia_INSTALL_PREFIX}"
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
)

ExternalProject_Get_Property(glsl-optimizer install_dir)
set(GLSL_Optimizer_HOME "${install_dir}" CACHE INTERNAL "")
