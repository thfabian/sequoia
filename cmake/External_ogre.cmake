##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                      S E Q U O I A
##
## This file is distributed under the MIT License (MIT). 
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

unset(_deps)
sequoia_add_optional_deps(_deps tbb boost ogredeps)

find_program(PATCH_BIN patch)
if(PATCH_BIN)
  set(patch_command ${PATCH_BIN} CMake/Dependencies.cmake ${CMAKE_SOURCE_DIR}/cmake/ogre-1.9.patch)
else()
  set(patch_command)
endif()

if(DEFINED BOOST_ROOT)
  list(APPEND ogre_cmake_args -DBOOST_ROOT:PATH=${BOOST_ROOT} -DBoost_NO_SYSTEM_PATHS:BOOL=ON)
endif()

if(DEFINED TBB_ROOT)
  list(APPEND ogre_cmake_args -DTBB_ROOT_DIR:PATH=${TBB_ROOT_DIR})
endif()

ExternalProject_Add(
  ogre
  DOWNLOAD_DIR ${download_dir}
  UPDATE_COMMAND ${update_command}
  PATCH_COMMAND ${patch_command}
  URL ${ogre_url}
  URL_MD5 ${ogre_md5}
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/ogre"
  INSTALL_DIR "${Sequoia_INSTALL_PREFIX}"
  CMAKE_ARGS
    ${ogre_cmake_args}
    -DOGRE_BUILD_SAMPLES=OFF
    -DOGRE_INSTALL_SAMPLES=OFF
    -DOGRE_DEPENDENCIES_DIR:PATH=${OGRE_DEPENDENCIES_DIR}
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
  DEPENDS
    ${_deps}
)

ExternalProject_Get_Property(ogre install_dir)
set(OGRE_HOME "${install_dir}" CACHE INTERNAL "")

list(APPEND Sequoia_THIRDPARTYLIBS_ARGS "-DOGRE_HOME:PATH=${OGRE_HOME}")

