##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                      S E Q U O I A
##
## This file is distributed under the MIT License (MIT). 
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

ExternalProject_Add(
  ogre
  DOWNLOAD_DIR ${download_dir}
  UPDATE_COMMAND ${update_command}
  URL ${ogre_url}
  URL_MD5 ${ogre_md5}
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/ogre"
  INSTALL_DIR "${Sequoia_INSTALL_PREFIX}"
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
)

ExternalProject_Get_Property(ogre install_dir)
set(OGRE_ROOT "${install_dir}" CACHE INTERNAL "")

list(APPEND Sequoia_THIRDPARTYLIBS_ARGS "-DOGRE_ROOT:PATH=${OGRE_ROOT}")

