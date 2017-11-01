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
include(SequoiaExportPackage)

# Set CMake arguments
set(sequoia_engine_cmake_args 
  ${SEQUOIA_EXTERNAL_CMAKE_ARGS} 
  -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
)

if(SEQUOIA_ASSERTS)
  set(SEQUOIA_ENGINE_ASSERTS ON)
endif()

foreach(option ${SEQUOIA_ENGINE_OPTIONS})
  list(APPEND sequoia_engine_cmake_args "-D${option}=${${option}}") 
endforeach()

# Compute dependencies
foreach(project zlib backward benchmark boost fmt glbinding glfw3 gli glm opencv tbb tinyobjloader 
                gtest cereal)
  sequoia_compute_optional_dependency(${project} sequoia_engine_deps) 
  sequoia_extract_cmake_package_args(${project} sequoia_engine_cmake_args)
endforeach()

set(sequoia_engine_source "${SEQUOIA_ENGINE_DIR}")
set(sequoia_engine_build "${CMAKE_BINARY_DIR}/sequoia-engine")

ExternalProject_Add(
  SequoiaEngine
  SOURCE_DIR "${sequoia_engine_source}"
  BINARY_DIR "${sequoia_engine_build}"
  INSTALL_DIR "${CMAKE_INSTALL_PREFIX}"
  CMAKE_ARGS ${sequoia_engine_cmake_args}
  DEPENDS ${sequoia_engine_deps}
)

# Create script to rerun CMake
sequoia_make_cmake_script(
  ${sequoia_engine_source}
  ${sequoia_engine_build} 
  ${sequoia_engine_cmake_args}
)

ExternalProject_Get_Property(SequoiaEngine install_dir)
set(SequoiaEngine_DIR "${install_dir}/cmake/sequoia-engine" CACHE INTERNAL "")

sequoia_export_package(
  PACKAGE SequoiaEngine 
  CMAKE_ARGS 
    "-DSequoiaEngine_DIR:PATH=${SequoiaEngine_DIR}" 
)
