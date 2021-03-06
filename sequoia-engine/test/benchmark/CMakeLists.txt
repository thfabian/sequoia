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

macro(sequoia_engine_add_benchmark SOURCE)
  get_filename_component(name ${SOURCE} NAME_WE)
  sequoia_add_executable(
    NAME SequoiaEngine${name}
    SOURCES ${SOURCE}
    DEPENDS SequoiaEngineUnittestStatic
            SequoiaEngineStatic
            ${SEQUOIA_ENGINE_TESTING_LIBRARIES}
    OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin/benchmark
  )
endmacro()

sequoia_engine_add_benchmark(BenchmarkUniformVariable.cpp)
sequoia_engine_add_benchmark(BenchmarkSceneNode.cpp)
sequoia_engine_add_benchmark(BenchmarkVertexAdapter.cpp)
sequoia_engine_add_benchmark(BenchmarkGLRenderer.cpp)

