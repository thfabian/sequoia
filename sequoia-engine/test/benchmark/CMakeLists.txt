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

macro(sequoia_add_benchmark SOURCE)
  get_filename_component(name ${SOURCE} NAME_WE)
  sequoia_add_executable(
    NAME Sequoia${name}
    SOURCES ${SOURCE}
    DEPENDS Sequoia
            ${SEQUOIA_EXTERNAL_LIBRARIES}
    OUTPUT_DIR benchmark
  )
endmacro()

sequoia_add_benchmark(BenchmarkUniformVariable.cpp)
sequoia_add_benchmark(GL/BenchmarkGLBuffer.cpp)

