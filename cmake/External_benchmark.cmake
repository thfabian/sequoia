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
  benchmark
  DOWNLOAD_DIR ${download_dir}
  URL ${benchmark_url}
  URL_MD5 ${benchmark_md5}
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/benchmark"
  INSTALL_DIR "${Sequoia_INSTALL_PREFIX}/benchmark"
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DCMAKE_CONFIGURATION_TYPES:STRING=${CMAKE_CONFIGURATION_TYPES}
    -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    -DBENCHMARK_ENABLE_TESTING:BOOL=OFF
)

ExternalProject_Get_Property(benchmark install_dir)
set(benchmark_DIR "${install_dir}/lib/cmake/benchmark" CACHE INTERNAL "")

list(APPEND Sequoia_THIRDPARTY_CMAKE_ARGS "-Dbenchmark_DIR:PATH=${benchmark_DIR}")
