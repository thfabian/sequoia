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

set(SPHINX_ROOT ${CMAKE_CURRENT_LIST_DIR})

#
# sequoia-cmake
#
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/sequoia-cmake-generate.cmake.in
               ${CMAKE_CURRENT_BINARY_DIR}/sequoia-cmake-generate.cmake @ONLY)
add_custom_target(sequoia-cmake
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/sequoia-cmake-generate.cmake
  COMMENT "Configuring sequoia-cmake" VERBATIM
)

#
# sphinx
#
add_custom_target(sphinx ALL
   COMMAND ${SPHINX_EXECUTABLE} -b html -d ${CMAKE_BINARY_DIR}/sphinx/doctrees 
           ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_BINARY_DIR}/sphinx/html
   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
   COMMENT "Generating sphinx documentation" VERBATIM
)

add_dependencies(sphinx sequoia-cmake)