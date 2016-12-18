##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                      S E Q U O I A
##
## This file is distributed under the MIT License (MIT). 
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

# Patch: https://bugs.launchpad.net/ubuntu/+source/gcc-5/+bug/1568899
find_program(PATCH_BIN patch)
if(PATCH_BIN)
  set(update_command 
      ${PATCH_BIN} src/SFML/Graphics/CMakeLists.txt -i ${CMAKE_SOURCE_DIR}/cmake/sfml.patch)
endif()

ExternalProject_Add(
  sfml
  DOWNLOAD_DIR ${download_dir}
  UPDATE_COMMAND ${update_command}
  URL ${sfml_url}
  URL_MD5 ${sfml_md5}
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/sfml"
  INSTALL_DIR "${Sequoia_INSTALL_PREFIX}"
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
)

ExternalProject_Get_Property(sfml install_dir)
set(SFML_ROOT "${install_dir}" CACHE INTERNAL "")

list(APPEND Sequoia_THIRDPARTYLIBS_ARGS "-DSFML_ROOT:PATH=${SFML_ROOT}")

