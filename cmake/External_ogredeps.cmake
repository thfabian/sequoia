##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                      S E Q U O I A
##
## This file is distributed under the MIT License (MIT). 
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

ExternalProject_Add(
  ogredeps
  DOWNLOAD_DIR ${download_dir}
  DOWNLOAD_NAME "ogredeps-v${ogredeps_version}.tar.gz"
  UPDATE_COMMAND ${update_command}
  URL ${ogredeps_url}
  URL_MD5 ${ogredeps_md5}
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/ogredeps"
  INSTALL_DIR "${Sequoia_INSTALL_PREFIX}"
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
)

ExternalProject_Get_Property(ogredeps install_dir)
set(OGRE_DEPENDENCIES_DIR "${install_dir}" CACHE INTERNAL "")

set(OIS_HOME ${OGRE_DEPENDENCIES_DIR})
list(APPEND Sequoia_THIRDPARTYLIBS_ARGS "-DOIS_HOME:PATH=${OIS_HOME}")
