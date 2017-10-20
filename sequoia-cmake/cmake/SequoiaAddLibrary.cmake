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
# .. _sequoia_add_library:
#
# sequoia_add_library
# -------------------
#
# Compile the given sources into an archive or object library. This will provide the CMake targets
# ``<NAME>Objects``, ``<NAME>Static`` and ``<NAME>Shared`` respectively. 
#
# .. code-block:: cmake
#
#   sequoia_add_library(NAME SOURCES DEPENDS [OUTPUT_DIR])
#
# ``NAME``
#   Name of the exectuable as well as the CMake target to build it.
# ``SOURCES``
#   List of source files making up the exectuable.
# ``DEPENDS``
#   List of external libraries and/or CMake targets to link against.
# ``OBJECT`` [optional]
#   Create a CMake object library.
# ``ARCHIVE`` [optional] 
#   Create a static and, if ``BUILD_SHARED_LIBS`` is ON, shared archive.
#
function(sequoia_add_library)  
  set(options OBJECT ARCHIVE)
  set(one_value_args NAME)
  set(multi_value_args SOURCES DEPENDS)
  cmake_parse_arguments(ARG "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})
  
  if(NOT("${ARG_UNPARSED_ARGUMENTS}" STREQUAL ""))
    message(FATAL_ERROR "invalid argument ${ARG_UNPARSED_ARGUMENTS}")
  endif()
  
  if(ARG_OBJECT)
   add_library(${ARG_NAME}Objects OBJECT ${ARG_SOURCES})
  endif()
  
  if(ARG_ARCHIVE)
    add_library(${ARG_NAME}Static STATIC ${ARG_SOURCES})
    target_link_libraries(${ARG_NAME}Static ${ARG_DEPENDS})
    set_target_properties(${ARG_NAME}Static PROPERTIES OUTPUT_NAME ${ARG_NAME})

    if(BUILD_SHARED_LIBS)
      add_library(${ARG_NAME}Shared SHARED ${ARG_SOURCES})
      target_link_libraries(${ARG_NAME}Shared ${ARG_DEPENDS})
      set_target_properties(${ARG_NAME}Shared PROPERTIES OUTPUT_NAME ${ARG_NAME})
    endif()
  endif()
endfunction()