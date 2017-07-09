##===------------------------------------------------------------------------------*- CMake -*-===##
##                         _____                        _
##                        / ____|                      (_)
##                       | (___   ___  __ _ _   _  ___  _  __ _
##                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
##                        ____) |  __/ (_| | |_| | (_) | | (_| |
##                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine (2016-2017)
##                                       | |
##                                       |_|
##
## This file is distributed under the MIT License (MIT).
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

ExternalProject_Add(
  tbb
  DOWNLOAD_DIR ${download_dir}
  URL ${tbb_url}
  URL_MD5 ${tbb_md5}
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/tbb"
  INSTALL_DIR "${Sequoia_INSTALL_PREFIX}/tbb"
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
)

ExternalProject_Get_Property(tbb install_dir)
set(TBB_ROOT_DIR "${install_dir}" CACHE INTERNAL "")

list(APPEND Sequoia_THIRDPARTYLIBS_ARGS "-DTBB_ROOT_DIR:PATH=${TBB_ROOT_DIR}")

