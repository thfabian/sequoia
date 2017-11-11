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

# sequoia-engine (doxygen)
include("${SEQUOIA_DOCS_ENGINE_DIR}/cmake/SequoiaEngineVersion.cmake")
set(SEQUOIA_ENGINE_DIR ${SEQUOIA_DOCS_ENGINE_DIR})
set(SEQUOIA_ENGINE_DOC_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SEQUOIA_ENGINE_VERSION ${SEQUOIA_ENGINE_VERSION})

set(sequoia_engine_doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/sequoia-engine-doxyfile.in)
set(sequoia_engine_doxyfile ${CMAKE_CURRENT_BINARY_DIR}/sequoia-engine-doxyfile)
configure_file(${sequoia_engine_doxyfile_in} ${sequoia_engine_doxyfile} @ONLY)

add_custom_target(sequoia-engine-doxygen
  COMMAND ${DOXYGEN_EXECUTABLE} ${sequoia_engine_doxyfile}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Generating sequoia-engine doxygen documentation" VERBATIM
  DEPENDS ${sequoia_engine_doxyfile} 
)

# sequoia-cmake
set(SEQUOIA_CMAKE_DOC_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/sequoia-cmake-generate.cmake.in
               ${CMAKE_CURRENT_BINARY_DIR}/sequoia-cmake-generate.cmake @ONLY)
add_custom_target(sequoia-cmake
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/sequoia-cmake-generate.cmake
  COMMENT "Configuring sequoia-cmake" VERBATIM
)

# sphinx
add_custom_target(sphinx
  COMMAND ${SPHINX_EXECUTABLE} -b html -d ${CMAKE_BINARY_DIR}/sphinx/doctrees 
         ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_BINARY_DIR}/sphinx/html
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Generating sphinx documentation" VERBATIM
  DEPENDS sequoia-cmake
)

# Copy HTML
add_custom_target(docs ALL
 # sphinx
 COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/sphinx/html 
                                            ${CMAKE_SOURCE_DIR}/html
 # sequoia-engine (doxygen)
 COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/sequoia-engine/html 
                                            ${CMAKE_SOURCE_DIR}/html/doxygen/sequoia-engine
 WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
 COMMENT "Generating HTML documentation" VERBATIM
 DEPENDS sphinx sequoia-engine-doxygen
)
