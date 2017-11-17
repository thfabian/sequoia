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
# sequoia_add_executable
# ----------------------
#
# Compile the given sources into a runnable executable (.exe).
#
# .. code-block:: cmake
#
#   sequoia_add_executable(NAME SOURCES DEPENDS [WIN32_APPLICATION] [OUTPUT_DIR] 
#                          [INSTALL_DESTINATION])
#
# ``NAME``
#   Name of the exectuable as well as the CMake target to build it.
# ``SOURCES``
#   List of source files making up the exectuable.
# ``DEPENDS``
#   List of external libraries and/or CMake targets to link against.
# ``WIN32_APPLICATION`` [optional]
#   Build an executable with a WinMain entry point on Win32.
# ``OUTPUT_DIR`` [optional]
#   Directory to place the exectuable (e.g ``${CMAKE_BINARY_DIR}/bin``).
# ``INSTALL_DESTINATION`` [optional]
#   Destition (relative to ``CMAKE_INSTALL_PREFIX``) to install the executable.
#
function(sequoia_add_executable)
  set(options WIN32_APPLICATION)
  set(one_value_args NAME OUTPUT_DIR INSTALL_DESTINATION)
  set(multi_value_args SOURCES DEPENDS)
  cmake_parse_arguments(ARG "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

  if(NOT("${ARG_UNPARSED_ARGUMENTS}" STREQUAL ""))
    message(FATAL_ERROR "sequoia_add_executable: invalid argument ${ARG_UNPARSED_ARGUMENTS}")
  endif()

  # Object libraries are trated as sources
  set(depends)
  set(sources ${ARG_SOURCES})
  foreach(lib ${ARG_DEPENDS})
    if(TARGET ${lib})
      get_target_property(type ${lib} TYPE)
      if("${type}" STREQUAL "OBJECT_LIBRARY")
        list(APPEND sources $<TARGET_OBJECTS:${lib}>)
      else()
        list(APPEND depends ${lib})
      endif()
    else()
      list(APPEND depends ${lib})
    endif()
  endforeach()
  
  # Create executable
  if(ARG_WIN32_APPLICATION)
    add_executable(${ARG_NAME} WIN32 ${sources})
  else()
    add_executable(${ARG_NAME} ${sources})
  endif()

  # Use folders in Visual Studio  
  if(MSVC)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    foreach(src ${ARG_SOURCES})
      get_filename_component(dir ${src} DIRECTORY)
      if(dir)
        set(group "Sources\\${dir}")
        source_group("${group}" FILES ${src})
      else()
        source_group("Sources" FILES ${src})
      endif()
    endforeach()
  endif()

  # Link executable
  target_link_libraries(${ARG_NAME} ${depends})

  if(ARG_OUTPUT_DIR)
    set_target_properties(${ARG_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${ARG_OUTPUT_DIR}")
  endif()

  if(ARG_INSTALL_DESTINATION)
    install(TARGETS ${ARG_NAME} 
            DESTINATION ${ARG_INSTALL_DESTINATION} 
            EXPORT ${ARG_NAME}Targets)
  endif()
endfunction()
