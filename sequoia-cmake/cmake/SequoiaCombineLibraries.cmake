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
# sequoia_combine_libraries
# -------------------------
#
# Combine multiple object libraries to a single static and, if ``BUILD_SHARED_LIBS`` is ON, shared 
# library. The CMake target of the library is ``<NAME>Static`` and ``<NAME>Shared`` respectively.
# This will also provide an install traget for the libraries as well as a package export via 
# ``<NAME>Targets``. However, the exported targets are *not* installed -- this needs to be done 
# manaully via ``install(EXPORT <NAME>Targets ...)``.
#
# .. code-block:: cmake
#
#   sequoia_combine_libraries(NAME OBJECTS INSTALL_DESTINATION [DEPENDS])
#
# ``NAME``
#   Name of the library.
# ``OBJECTS``
#   Object libraries to combine (see :ref:`sequoia_add_library`).
# ``INSTALL_DESTINATION``
#   Destition (relative to ``CMAKE_INSTALL_PREFIX``) to install the libraries.
# ``VERSION``
#   Version of the library (only used for shared objects).
# ``DEPENDS`` [optional]
#   List of external libraries and/or CMake targets treated as dependencies of the library.
#
function(sequoia_combine_libraries)
  set(one_value_args NAME INSTALL_DESTINATION VERSION)
  set(multi_value_args OBJECTS DEPENDS)
  cmake_parse_arguments(ARG "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})
  
  if(NOT("${ARG_UNPARSED_ARGUMENTS}" STREQUAL ""))
    message(FATAL_ERROR "sequoia_combine_libraries: invalid argument ${ARG_UNPARSED_ARGUMENTS}")
  endif()

  if(NOT("${ARG_OBJECTS}" STREQUAL ""))
    set(object_sources)
    foreach(object ${ARG_OBJECTS})
      list(APPEND object_sources $<TARGET_OBJECTS:${object}>)
    endforeach()
  endif()

  # Add static library
  add_library(${ARG_NAME}Static STATIC ${object_sources})
  target_link_libraries(${ARG_NAME}Static LINK_PUBLIC ${ARG_DEPENDS})

  set_target_properties(${ARG_NAME}Static PROPERTIES OUTPUT_NAME "${ARG_NAME}")
  set_target_properties(${ARG_NAME}Static PROPERTIES LINK_INTERFACE_LIBRARIES "${ARG_DEPENDS}")

  if(ARG_VERSION)
    set_target_properties(${ARG_NAME}Static PROPERTIES VERSION "${ARG_VERSION}")
  endif()

  install(TARGETS ${ARG_NAME}Static 
          DESTINATION ${ARG_INSTALL_DESTINATION} 
          EXPORT ${ARG_NAME}Targets)
  
  # Add shared library
  if(BUILD_SHARED_LIBS)
    add_library(${ARG_NAME}Shared SHARED ${object_sources})
    target_link_libraries(${ARG_NAME}Shared LINK_PUBLIC ${ARG_DEPENDS})

    set_target_properties(${ARG_NAME}Shared PROPERTIES OUTPUT_NAME "${ARG_NAME}")
    set_target_properties(${ARG_NAME}Shared PROPERTIES LINK_INTERFACE_LIBRARIES "${ARG_DEPENDS}")

    if(ARG_VERSION)
      set_target_properties(${ARG_NAME}Shared PROPERTIES VERSION "${ARG_VERSION}")
      set_target_properties(${ARG_NAME}Shared PROPERTIES SOVERSION "${ARG_VERSION}")
    endif()

    install(TARGETS ${ARG_NAME}Shared 
            DESTINATION ${ARG_INSTALL_DESTINATION} 
            EXPORT ${ARG_NAME}Targets)
  endif()
endfunction()
