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
  DOWNLOAD_DIR "${SEQUOIA_EXTERNAL_DOWNLOAD_DIR}/benchmark"
  URL ${benchmark_url}
  URL_MD5 ${benchmark_md5}
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/benchmark"
  INSTALL_DIR "${SEQUOIA_EXTERNAL_INSTALL_PREFIX}/benchmark"
  CMAKE_ARGS
    ${SEQUOIA_EXTERNAL_CMAKE_ARGS}
    ${SEQUOIA_EXTERNAL_PROJECTS_CMAKE_ARGS}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    -DBENCHMARK_ENABLE_TESTING:BOOL=OFF
)

ExternalProject_Get_Property(benchmark install_dir)
set(benchmark_DIR "${install_dir}/lib/cmake/benchmark" CACHE INTERNAL "")

sequoia_export_package(
  PACKAGE benchmark 
  CMAKE_ARGS 
    "-Dbenchmark_DIR:PATH=${benchmark_DIR}"
)