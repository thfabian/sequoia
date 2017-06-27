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

#.rst:
# FindFreeImage
# -------------
#
# This script locates FreeImage. This script makes use of the standard find_package arguments of
# ``VERSION``, ``REQUIRED`` and ``QUIET``. FREEIMAGE_FOUND will report if an acceptable version of
# FreeImage was found.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# Defines the following variables:
#
#   FREEIMAGE_FOUND               - System has the FreeImage.
#   FREEIMAGE_INCLUDE_DIRS        - The location of FreeImage headers
#   FREEIMAGE_LIBRARIES           - The location of FreeImage library
#
# Hints
# ^^^^^
#
# Set ``FREEIMAGE_ROOT`` to a directory that contains a FREEIMAGE installation.

include(FindPackageHandleStandardArgs)

#
# Parse FREEIMAGE_ROOT
#
set(FREEIMAGE_ROOT_ENV $ENV{FREEIMAGE_ROOT})
if(FREEIMAGE_ROOT_ENV)
  set(FREEIMAGE_ROOT ${FREEIMAGE_ROOT_ENV} CACHE PATH "FreeImage install path.")
endif()

if(NOT(FREEIMAGE_ROOT))
  find_path(FREEIMAGE_ROOT NAMES include/FreeImage/FreeImage.h)
else()
  get_filename_component(_FREEIMAGE_ROOT_ABSOLUTE ${FREEIMAGE_ROOT} ABSOLUTE)
  set(FREEIMAGE_ROOT ${_FREEIMAGE_ROOT_ABSOLUTE} CACHE PATH "FreeImage install path.")
endif()

#
# Default version is 3.15.3
#
if(NOT FreeImage_FIND_VERSION)
  if(NOT FreeImage_FIND_VERSION_MAJOR)
    set(FreeImage_FIND_VERSION_MAJOR 3)
  endif()
  if(NOT FreeImage_FIND_VERSION_MINOR)
    set(FreeImage_FIND_VERSION_MINOR 15)
  endif()
  if(NOT FreeImage_FIND_VERSION_PATCH)
    set(FreeImage_FIND_VERSION_PATCH 3)
  endif()
  set(FreeImage_FIND_VERSION 
      "${FreeImage_FIND_VERSION_MAJOR}.${FreeImage_FIND_VERSION_MINOR}.${FreeImage_FIND_VERSION_PATCH}")
endif()

#===---------------------------------------------------------------------------------------------===
#   Find FreeImage headers
#====--------------------------------------------------------------------------------------------===
if(NOT(FREEIMAGE_INCLUDE_DIRS))
  find_path(FREEIMAGE_INCLUDE_DIRS NAMES FreeImage/FreeImage.h HINTS ${FREEIMAGE_ROOT}/include)
endif()

#===---------------------------------------------------------------------------------------------===
#   Read version from FreeImage/FreeImage-version.h
#====--------------------------------------------------------------------------------------------===
if(FREEIMAGE_INCLUDE_DIRS AND NOT(FREEIMAGE_VERSION))
  file(READ ${FREEIMAGE_INCLUDE_DIRS}/FreeImage/FreeImage.h _CONFIG_FILE)

  # Get version  
  string(REGEX MATCH "define[ \t]+FREEIMAGE_MAJOR_VERSION[ \t]+([0-9]+)" _MAJOR "${_CONFIG_FILE}")
  set(FREEIMAGE_MAJOR_VERSION "${CMAKE_MATCH_1}")
  string(REGEX MATCH "define[ \t]+FREEIMAGE_MINOR_VERSION[ \t]+([0-9]+)" _MINOR "${_CONFIG_FILE}")
  set(FREEIMAGE_MINOR_VERSION "${CMAKE_MATCH_1}")
  string(REGEX MATCH "define[ \t]+FREEIMAGE_RELEASE_SERIAL[ \t]+([0-9]+)" _PATCH "${_CONFIG_FILE}")
  set(FREEIMAGE_PATCH_VERSION "${CMAKE_MATCH_1}")

  set(FREEIMAGE_VERSION 
      "${FREEIMAGE_MAJOR_VERSION}.${FREEIMAGE_MINOR_VERSION}.${FREEIMAGE_PATCH_VERSION}")
endif()

#===---------------------------------------------------------------------------------------------===
#   Find FreeImage library
#====--------------------------------------------------------------------------------------------===
if(NOT(FREEIMAGE_LIBRARIES))
  find_library(FREEIMAGE_LIBRARIES NAMES FreeImage HINTS ${FREEIMAGE_ROOT}/lib)
endif()

#===---------------------------------------------------------------------------------------------===
#   Report result 
#====--------------------------------------------------------------------------------------------===
find_package_handle_standard_args(
  FreeImage
  FAIL_MESSAGE "Could NOT find FreeImage. (Try setting FREEIMAGE_ROOT in the env)"
  REQUIRED_VARS FREEIMAGE_LIBRARIES FREEIMAGE_INCLUDE_DIRS
  VERSION_VAR FREEIMAGE_VERSION
)

mark_as_advanced(
  FREEIMAGE_INCLUDE_DIRS
  FREEIMAGE_LIBRARIES
)

if(NOT(FREEIMAGE_FOUND) AND FreeImage_FIND_REQUIRED EQUAL 1)
  message(FATAL_ERROR "Could NOT find FREEIMAGE. (Try setting FREEIMAGE_ROOT in the env)")
endif()

