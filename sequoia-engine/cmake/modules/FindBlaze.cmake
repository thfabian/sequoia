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
# FindBlaze
# ---------
#
# This script locates blaze. This script makes use of the standard find_package arguments of
# ``VERSION``, ``REQUIRED`` and ``QUIET``. BLAZE_FOUND will report if an acceptable version of
# blaze was found.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# Defines the following variables:
#
#   BLAZE_FOUND               - System has the blaze headers
#   BLAZE_INCLUDE_DIRS        - The location of BLAZE headers (i.e the header Blaze.h is in 
#                               ${BLAZE_INCLUDE_DIRS}/blaze/Blaze.h)
#
#
# Hints
# ^^^^^
#
# Set ``BLAZE_ROOT`` to a directory that contains a BLAZE installation, or directly set 
# ``BLAZE_INCLUDE_DIRS``.

include(FindPackageHandleStandardArgs)

#
# Parse BLAZE_ROOT
#
set(BLAZE_ROOT_ENV $ENV{BLAZE_ROOT})
if(BLAZE_ROOT_ENV)
  set(BLAZE_ROOT ${BLAZE_ROOT_ENV} CACHE PATH "blaze install path.")
endif()

if(NOT(BLAZE_ROOT))
  find_path(BLAZE_ROOT NAMES blaze/Blaze.h HINTS ${BLAZE_INCLUDE_DIRS})
else()
  get_filename_component(_BLAZE_ROOT_ABSOLUTE ${BLAZE_ROOT} ABSOLUTE)
  set(BLAZE_ROOT ${_BLAZE_ROOT_ABSOLUTE} CACHE PATH "blaze install path.")
endif()

#
# Default version is 3.1
#
if(NOT Blaze_FIND_VERSION)
  if(NOT Blaze_FIND_VERSION_MAJOR)
    set(Blaze_FIND_VERSION_MAJOR 3)
  endif(NOT Blaze_FIND_VERSION_MAJOR)
  if(NOT Blaze_FIND_VERSION_MINOR)
    set(Blaze_FIND_VERSION_MINOR 1)
  endif(NOT Blaze_FIND_VERSION_MINOR)
  set(Blaze_FIND_VERSION "${Blaze_FIND_VERSION_MAJOR}.${Blaze_FIND_VERSION_MINOR}")
endif(NOT Blaze_FIND_VERSION)

#===---------------------------------------------------------------------------------------------===
#   Find blaze headers
#====--------------------------------------------------------------------------------------------===
if(NOT(BLAZE_INCLUDE_DIRS))
  find_path(BLAZE_INCLUDE_DIRS NAMES blaze/Blaze.h HINTS ${BLAZE_ROOT})
endif()

#===---------------------------------------------------------------------------------------------===
#   Read version from blaze/system/Version.h
#====--------------------------------------------------------------------------------------------===
if(BLAZE_INCLUDE_DIRS AND NOT(BLAZE_VERSION))
  file(READ ${BLAZE_INCLUDE_DIRS}/blaze/system/Version.h _CONFIG_FILE)

  # Get version  
  string(REGEX MATCH "define[ \t]+BLAZE_MAJOR_VERSION[ \t]+([0-9]+)" _MAJOR "${_CONFIG_FILE}")
  set(BLAZE_MAJOR_VERSION "${CMAKE_MATCH_1}")
  string(REGEX MATCH "define[ \t]+BLAZE_MINOR_VERSION[ \t]+([0-9]+)" _MINOR "${_CONFIG_FILE}")
  set(BLAZE_MINOR_VERSION "${CMAKE_MATCH_1}")

  set(BLAZE_VERSION "${BLAZE_MAJOR_VERSION}.${BLAZE_MINOR_VERSION}")
endif()

#===---------------------------------------------------------------------------------------------===
#   Report result 
#====--------------------------------------------------------------------------------------------===
find_package_handle_standard_args(
  BLAZE
  FAIL_MESSAGE "Could NOT find BLAZE. (Try setting BLAZE_ROOT in the env)"
  REQUIRED_VARS  
    BLAZE_ROOT
    BLAZE_INCLUDE_DIRS
  VERSION_VAR BLAZE_VERSION
)

mark_as_advanced(BLAZE_INCLUDE_DIRS)

if(NOT(BLAZE_FOUND) AND Blaze_FIND_REQUIRED EQUAL 1)
  message(FATAL_ERROR "Could NOT find BLAZE. (Try setting BLAZE_ROOT in the env)")
endif()

