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
include(SequoiaMakeCMakeScript)

# Set CMake arguments
set(sequoia_docs_cmake_args 
  ${SEQUOIA_EXTERNAL_CMAKE_ARGS} 
  -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
)

set(SEQUOIA_DOCS_CMAKE_DIR "${SEQUOIA_CMAKE_DIR}")
set(SEQUOIA_DOCS_ENGINE_DIR "${SEQUOIA_ENGINE_DIR}")

foreach(option ${SEQUOIA_DOCS_OPTIONS})
  list(APPEND sequoia_docs_cmake_args "-D${option}=${${option}}") 
endforeach()

set(sequoia_docs_source "${SEQUOIA_DOCS_DIR}")
set(sequoia_docs_build "${CMAKE_BINARY_DIR}/sequoia-docs")

ExternalProject_Add(
  SequoiaDocs
  SOURCE_DIR "${sequoia_docs_source}"
  BINARY_DIR "${sequoia_docs_build}"
  INSTALL_DIR "${CMAKE_INSTALL_PREFIX}"
  CMAKE_ARGS ${sequoia_docs_cmake_args}
)

# Create script to rerun CMake
sequoia_make_cmake_script(
  ${sequoia_docs_source}
  ${sequoia_docs_build} 
  ${sequoia_docs_cmake_args}
)
