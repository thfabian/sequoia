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

include(ExternalProject)
include(SequoiaAddOptionalDeps)
include(SequoiaMakeCMakeScript)

set(sequoia_examples_cmake_args ${SEQUOIA_EXTERNAL_CMAKE_ARGS})

message("${sequoia_examples_cmake_args}")

# if(SEQUOIA_ASSERTS)
#   set(SEQUOIA_ENGINE_ASSERTS ON)
# endif()

# foreach(option ${SEQUOIA_ENGINE_OPTIONS})
#   list(APPEND sequoia_engine_cmake_args "-D${option}=${${option}}")  
# endforeach()
# list(APPEND sequoia_engine_cmake_args "-DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")

# set(sequoia_engine_source "${SEQUOIA_ENGINE_DIR}")
# set(sequoia_engine_build "${CMAKE_BINARY_DIR}/sequoia-engine")

# sequoia_add_optional_deps(sequoia_engine_deps "${SEQUOIA_EXTERNAL_PROJECTS}")

# ExternalProject_Add(
#   SequoiaEngine
#   SOURCE_DIR "${sequoia_engine_source}"
#   BINARY_DIR "${sequoia_engine_build}"
#   INSTALL_DIR "${CMAKE_INSTALL_PREFIX}"
#   CMAKE_ARGS ${sequoia_engine_cmake_args}
#   DEPENDS ${sequoia_engine_deps}
# )

# sequoia_make_cmake_script(
#   ${sequoia_engine_source}
#   ${sequoia_engine_build} 
#   ${sequoia_engine_cmake_args}
# )

# ExternalProject_Get_Property(SequoiaEngine install_dir)
# set(SequoiaEngine_DIR "${install_dir}/cmake/sequoia-engine" CACHE INTERNAL "")