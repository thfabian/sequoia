##===------------------------------------------------------------------------------*- CMake -*-===##
##                         _____                        _       
##                        / ____|                      (_)      
##                       | (___   ___  __ _ _   _  ___  _  __ _ 
##                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
##                        ____) |  __/ (_| | |_| | (_) | | (_| |
##                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine
##                                       | |                    
##                                       |_| 
##
## This file is distributed under the MIT License (MIT).
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

#.rst:
# Findglbinding
# -------------
#
# This script locates glbinding. This script makes use of the standard find_package arguments of
# ``VERSION``, ``REQUIRED`` and ``QUIET``. GLBINDING_FOUND will report if an acceptable version of
# glbinding was found.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# Defines the following variables:
#
#   GLBINDING_FOUND               - System has the glbinding.
#   GLBINDING_INCLUDE_DIRS        - The location of glbinding headers
#   GLBINDING_LIBRARIES           - The location of glbinding library
#
# Hints
# ^^^^^
#
# Set ``GLBINDING_ROOT`` to a directory that contains a GLBINDING installation, or directly set 
# ``GLBINDING_INCLUDE_DIRS``.

include(FindPackageHandleStandardArgs)

#
# Parse GLBINDING_ROOT
#
set(GLBINDING_ROOT_ENV $ENV{GLBINDING_ROOT})
if(GLBINDING_ROOT_ENV)
  set(GLBINDING_ROOT ${GLBINDING_ROOT_ENV} CACHE PATH "glbinding install path.")
endif()

if(NOT(GLBINDING_ROOT))
  find_path(GLBINDING_ROOT NAMES include/glbinding/Binding.h)
else()
  get_filename_component(_GLBINDING_ROOT_ABSOLUTE ${GLBINDING_ROOT} ABSOLUTE)
  set(GLBINDING_ROOT ${_GLBINDING_ROOT_ABSOLUTE} CACHE PATH "glbinding install path.")
endif()

#
# Default version is 2.1.1
#
if(NOT glbinding_FIND_VERSION)
  if(NOT glbinding_FIND_VERSION_MAJOR)
    set(glbinding_FIND_VERSION_MAJOR 2)
  endif()
  if(NOT glbinding_FIND_VERSION_MINOR)
    set(glbinding_FIND_VERSION_MINOR 1)
  endif()
  if(NOT glbinding_FIND_VERSION_PATCH)
    set(glbinding_FIND_VERSION_PATCH 1)
  endif()
  set(glbinding_FIND_VERSION 
      "${glbinding_FIND_VERSION_MAJOR}.${glbinding_FIND_VERSION_MINOR}.${glbinding_FIND_VERSION_PATCH}")
endif()

#===---------------------------------------------------------------------------------------------===
#   Find glbinding headers
#====--------------------------------------------------------------------------------------------===
if(NOT(GLBINDING_INCLUDE_DIRS))
  find_path(GLBINDING_INCLUDE_DIRS NAMES glbinding/Binding.h HINTS ${GLBINDING_ROOT}/include)
endif()

#===---------------------------------------------------------------------------------------------===
#   Read version from glbinding/glbinding-version.h
#====--------------------------------------------------------------------------------------------===
if(GLBINDING_INCLUDE_DIRS AND NOT(GLBINDING_VERSION))
  file(READ ${GLBINDING_INCLUDE_DIRS}/glbinding/glbinding-version.h _CONFIG_FILE)

  # Get version  
  string(REGEX MATCH "define[ \t]+GLBINDING_VERSION_MAJOR[ \t]+\"([0-9]+)\"" _MAJOR "${_CONFIG_FILE}")
  set(GLBINDING_MAJOR_VERSION "${CMAKE_MATCH_1}")
  string(REGEX MATCH "define[ \t]+GLBINDING_VERSION_MINOR[ \t]+\"([0-9]+)\"" _MINOR "${_CONFIG_FILE}")
  set(GLBINDING_MINOR_VERSION "${CMAKE_MATCH_1}")
  string(REGEX MATCH "define[ \t]+GLBINDING_VERSION_PATCH[ \t]+\"([0-9]+)\"" _PATCH "${_CONFIG_FILE}")
  set(GLBINDING_PATCH_VERSION "${CMAKE_MATCH_1}")

  set(GLBINDING_VERSION 
      "${GLBINDING_MAJOR_VERSION}.${GLBINDING_MINOR_VERSION}.${GLBINDING_PATCH_VERSION}")
endif()

#===---------------------------------------------------------------------------------------------===
#   Find glbinding library
#====--------------------------------------------------------------------------------------------===
if(NOT(GLBINDING_LIBRARIES))
  find_library(GLBINDING_LIBRARIES NAMES glbinding HINTS ${GLBINDING_ROOT}/lib)
endif()

#===---------------------------------------------------------------------------------------------===
#   Report result 
#====--------------------------------------------------------------------------------------------===
find_package_handle_standard_args(
  glbinding
  FAIL_MESSAGE "Could NOT find glbinding. (Try setting GLBINDING_ROOT in the env)"
  REQUIRED_VARS  
    GLBINDING_ROOT
    GLBINDING_INCLUDE_DIRS
    GLBINDING_LIBRARIES
  VERSION_VAR GLBINDING_VERSION
)

mark_as_advanced(
  GLBINDING_INCLUDE_DIRS
  GLBINDING_LIBRARIES
)

if(NOT(GLBINDING_FOUND) AND glbinding_FIND_REQUIRED EQUAL 1)
  message(FATAL_ERROR "Could NOT find GLBINDING. (Try setting GLBINDING_ROOT in the env)")
endif()

