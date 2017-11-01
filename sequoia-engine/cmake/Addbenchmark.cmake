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

find_package(benchmark NO_MODULE REQUIRED)

get_property(benchmark_INCLUDE_DIRS TARGET benchmark::benchmark PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
get_property(benchmark_EXTERNAL_LIBRARIES TARGET benchmark::benchmark PROPERTY INTERFACE_LINK_LIBRARIES)
get_property(benchmark_LIBRARY TARGET benchmark::benchmark PROPERTY LOCATION)

sequoia_export_package(
  NAME benchmark
  FOUND ${benchmark_FOUND} 
  LIBRARIES ${benchmark_LIBRARY} ${benchmark_EXTERNAL_LIBRARIES}
  INCLUDE_DIRS ${benchmark_INCLUDE_DIRS}
)
