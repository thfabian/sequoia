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

include(CMakeParseArguments)
include(SequoiaAddUnittest)

macro(sequoia_engine_add_unittest)
  cmake_parse_arguments(ARG "" "NAME" "SOURCES" ${ARGN})

  if(NOT("${ARG_UNPARSED_ARGUMENTS}" STREQUAL ""))
    message(FATAL_ERROR "sequoia_engine_add_unittest: invalid argument ${ARG_UNPARSED_ARGUMENTS}")
  endif()

  sequoia_add_unittest(
    NAME ${ARG_NAME}
    SOURCES ${ARG_SOURCES}
    DEPENDS SequoiaEngineUnittestStatic
            SequoiaEngineStatic
            ${SEQUOIA_ENGINE_TESTING_LIBRARIES}
    OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin/unittest
    GTEST_ARGS --gtest_color=yes
  )
endmacro()

add_subdirectory(Core)
add_subdirectory(Math)
add_subdirectory(Game)
add_subdirectory(Render)
