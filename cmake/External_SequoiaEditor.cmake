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
set(sequoia_editor_cmake_args 
  ${SEQUOIA_EXTERNAL_CMAKE_ARGS} 
  -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
)

if(SEQUOIA_ASSERTS)
  set(SEQUOIA_EDITOR_ASSERTS ON)
endif()

foreach(option ${SEQUOIA_EDITOR})
  list(APPEND sequoia_editor_cmake_args "-D${option}=${${option}}") 
endforeach()

# Compute dependencies
foreach(project SequoiaEngine gtest)
  sequoia_compute_optional_dependency(${project} sequoia_editor_deps) 
  sequoia_extract_cmake_package_args(${project} sequoia_editor_cmake_args)
endforeach()

set(sequoia_editor_source "${SEQUOIA_EDITOR_DIR}")
set(sequoia_editor_build "${CMAKE_BINARY_DIR}/sequoia-editor")

ExternalProject_Add(
  SequoiaEditor
  SOURCE_DIR "${sequoia_editor_source}"
  BINARY_DIR "${sequoia_editor_build}"
  INSTALL_DIR "${CMAKE_INSTALL_PREFIX}"
  INSTALL_COMMAND ""
  CMAKE_ARGS ${sequoia_editor_cmake_args}
  DEPENDS ${sequoia_editor_deps}
)

# Create script to rerun CMake
sequoia_make_cmake_script(
  ${sequoia_editor_source}
  ${sequoia_editor_build} 
  ${sequoia_editor_cmake_args}
)
