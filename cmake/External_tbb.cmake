##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                      S E Q U O I A
##
## This file is distributed under the MIT License (MIT). 
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

if(BUILD_SHARED_LIBS)
  list(APPEND tbb_extra_args "-DTBB_BUILD_STATIC=OFF" "-DTBB_BUILD_SHARED=ON")
else()
  list(APPEND tbb_extra_args "-DTBB_BUILD_STATIC=ON" "-DTBB_BUILD_SHARED=OFF")
endif()

ExternalProject_Add(
  tbb
  DOWNLOAD_DIR ${download_dir}
  URL ${tbb_url}
  URL_MD5 ${tbb_md5}
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/tbb"
  INSTALL_DIR "${Sequoia_INSTALL_PREFIX}"
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    ${tbb_extra_args}
)

ExternalProject_Get_Property(tbb install_dir)
set(TBB_ROOT_DIR "${install_dir}" CACHE INTERNAL "")

list(APPEND Sequoia_THIRDPARTYLIBS_ARGS "-DTBB_ROOT_DIR:PATH=${TBB_ROOT_DIR}")

