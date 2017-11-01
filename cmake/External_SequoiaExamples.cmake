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
include(SequoiaComputeOptionalDependency)
include(SequoiaExtractCMakePackageArgs)
include(SequoiaMakeCMakeScript)

# Set CMake arguments
set(sequoia_examples_cmake_args 
  ${SEQUOIA_EXTERNAL_CMAKE_ARGS} 
  -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
)

if(SEQUOIA_ASSERTS)
  set(SEQUOIA_EXAMPLES_ASSERTS ON)
endif()

foreach(option ${SEQUOIA_EXAMPLES})
  list(APPEND sequoia_examples_cmake_args "-D${option}=${${option}}") 
endforeach()

# Compute dependencies
foreach(project SequoiaEngine gtest)
  sequoia_compute_optional_dependency(${project} sequoia_examples_deps) 
  sequoia_extract_cmake_package_args(${project} sequoia_examples_cmake_args)
endforeach()

set(sequoia_examples_source "${SEQUOIA_EXAMPLES_DIR}")
set(sequoia_examples_build "${CMAKE_BINARY_DIR}/sequoia-examples")

ExternalProject_Add(
  SequoiaExamples
  SOURCE_DIR "${sequoia_examples_source}"
  BINARY_DIR "${sequoia_examples_build}"
  INSTALL_DIR "${CMAKE_INSTALL_PREFIX}"
  INSTALL_COMMAND ""
  CMAKE_ARGS ${sequoia_examples_cmake_args}
  DEPENDS ${sequoia_examples_deps}
)

# Create script to rerun CMake
sequoia_make_cmake_script(
  ${sequoia_examples_source}
  ${sequoia_examples_build} 
  ${sequoia_examples_cmake_args}
)
