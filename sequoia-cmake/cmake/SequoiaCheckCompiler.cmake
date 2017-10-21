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

include(CMakeParseArguments)

#.rst:
# sequoia_check_compiler
# ----------------------
#
# Check compliance of the compiler in use. 
#
# .. code-block:: cmake
#
#   sequoia_check_compiler(GCC CLANG MSVC INTEL)
#
# ``GCC``
#   Minimum version of GCC.
# ``CLANG``
#   Minimum version of Clang.
# ``INTEL``
#   Minimum version of Intel's ICC.
# ``MSVC``
#   Minimum version of Visual Studio.
#
function(sequoia_check_compiler)
  set(options)
  set(one_value_args GCC CLANG MSVC INTEL)
  cmake_parse_arguments(ARG "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
      if(DEFINED ARG_CLANG AND ${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS "{ARG_CLANG}")
          message(WARNING "
      ### You appear to be using Clang ${CMAKE_CXX_COMPILER_VERSION}, which is known
      ### to be unable to compile Sequoia. Consider switching to
      ### Clang >= {ARG_CLANG}. If it is already installed on your
      ### system, you can tell CMake about it with
      ###
      ###     cmake .. -DCMAKE_CXX_COMPILER=/path/to/clang
          ")
      endif()
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
      if(DEFINED ARG_INTEL AND ${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS "${ARG_INTEL}")
          message(WARNING "
      ### You appear to be using Intel's ICC ${CMAKE_CXX_COMPILER_VERSION}, which is known
      ### to be unable to compile Sequoia. Consider switching to
      ### ICC >= ${ARG_INTEL}. If it is already installed on your
      ### system, you can tell CMake about it with
      ###
      ###     cmake .. -DCMAKE_CXX_COMPILER=/path/to/icpc
          ")
      endif()
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
      if(DEFINED ARG_GCC AND ${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS "${ARG_GCC}")
          message(WARNING "
      ### You appear to be using GCC ${CMAKE_CXX_COMPILER_VERSION}, which is known to be
      ### unable to compile Sequoia. Only GCC >= ${ARG_GCC} is supported.
      ### Consider using a more recent GCC or switching to Clang.
      ### If a more recent compiler is already installed on your
      ### system, you can tell CMake to use it with
      ###
      ###     cmake .. -DCMAKE_CXX_COMPILER=/path/to/g++
          ")
      endif()
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
      if(DEFINED ARG_MSVC AND ${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS "${ARG_MSVC}")
          message(WARNING "
      ### You appear to be using Visual Studio ${CMAKE_CXX_COMPILER_VERSION}, 
      ### which is known to be unable to compile Sequoia. 
      ### Only Visual Studio 14 2015 is currently supported.
          ")
      endif()
  else()
      message(WARNING "
      ### You appear to be using a compiler that is not yet tested with Sequoia Engine.
      ")
  endif()

endfunction()
