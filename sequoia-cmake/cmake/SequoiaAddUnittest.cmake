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

include(SequoiaIncludeGuard)
sequoia_include_guard()

include(SequoiaAddExecutable)

#.rst:
# sequoia_add_unittest
# --------------------
#
# Compile the given objects into a runnable unittest executable (.exe) and register it within CTest.
# Note that to enable CTest you need to call the builtin command ``enable_testing()`` in the source 
# root.
#
# .. code-block:: cmake
#
#   sequoia_add_unittest(NAME SOURCES DEPENDS [OUTPUT_DIR GTEST_ARGS])
#
# ``NAME``
#   Name of the unittest exectuable as well as the CMake target to build it.
# ``SOURCES``
#   List of source files making up the exectuable.
# ``DEPENDS``
#   List of external libraries and/or CMake targets to link against.
# ``OUTPUT_DIR`` [optional]
#   Directory to place the exectuable (e.g ``${CMAKE_BINARY_DIR}/bin``).
# ``GTEST_ARGS`` [optional]
#   Arguments passed to the created GTest exectuable (e.g ``--gtest_color=yes``)
#
function(sequoia_add_unittest)
  set(one_value_args NAME OUTPUT_DIR)
  set(multi_value_args SOURCES DEPENDS GTEST_ARGS)
  cmake_parse_arguments(ARG "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

  if(NOT("${ARG_UNPARSED_ARGUMENTS}" STREQUAL ""))
    message(FATAL_ERROR "sequoia_add_unittest: invalid argument ${ARG_UNPARSED_ARGUMENTS}")
  endif()

  sequoia_add_executable(
    NAME ${ARG_NAME} 
    SOURCES ${ARG_SOURCES} 
    DEPENDS ${ARG_DEPENDS}
    OUTPUT_DIR ${ARG_OUTPUT_DIR}
  )  

  add_test(NAME CTest-${ARG_NAME} COMMAND $<TARGET_FILE:${ARG_NAME}> ${ARG_GTEST_ARGS})
endfunction()
