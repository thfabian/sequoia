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
# Findgli
# ---------
#
# This script locates gli. This script makes use of the standard find_package arguments of
# ``VERSION``, ``REQUIRED`` and ``QUIET``. GLI_FOUND will report if an acceptable version of
# gli was found.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# Defines the following variables:
#
#   GLI_FOUND               - System has the gli headers
#   GLI_INCLUDE_DIRS        - The location of GLI headers (i.e the header gli.hpp is in 
#                             ${GLI_INCLUDE_DIRS}/gli/gli.hpp)
#   GLI_VERSION             - Version of gli
#
# Hints
# ^^^^^
#
# Set ``GLI_ROOT`` to a directory that contains a GLI installation, or directly set 
# ``GLI_INCLUDE_DIRS``.

include(FindPackageHandleStandardArgs)

#
# Parse GLI_ROOT
#
set(GLI_ROOT_ENV $ENV{GLI_ROOT})
if(GLI_ROOT_ENV)
  set(GLI_ROOT ${GLI_ROOT_ENV} CACHE PATH "gli install path.")
endif()

if(NOT(GLI_ROOT))
  find_path(GLI_ROOT NAMES gli/gli.hpp HINTS ${GLI_INCLUDE_DIRS})
else()
  get_filename_component(_GLI_ROOT_ABSOLUTE ${GLI_ROOT} ABSOLUTE)
  set(GLI_ROOT ${_GLI_ROOT_ABSOLUTE} CACHE PATH "gli install path.")
endif()

#
# Default version is 0.8
#
if(NOT gli_FIND_VERSION)
  if(NOT gli_FIND_VERSION_MAJOR)
    set(gli_FIND_VERSION_MAJOR 0)
  endif(NOT gli_FIND_VERSION_MAJOR)
  if(NOT gli_FIND_VERSION_MINOR)
    set(gli_FIND_VERSION_MINOR 8)
  endif(NOT gli_FIND_VERSION_MINOR)
  set(gli_FIND_VERSION "${gli_FIND_VERSION_MAJOR}.${gli_FIND_VERSION_MINOR}")
endif(NOT gli_FIND_VERSION)

#===---------------------------------------------------------------------------------------------===
#   Find gli headers
#====--------------------------------------------------------------------------------------------===
if(NOT(GLI_INCLUDE_DIRS))
  find_path(GLI_INCLUDE_DIRS NAMES gli/gli.hpp HINTS ${GLI_ROOT})
endif()

#===---------------------------------------------------------------------------------------------===
#   Read version from gli/gli.hpp
#====--------------------------------------------------------------------------------------------===
if(GLI_INCLUDE_DIRS AND NOT(GLI_VERSION))
  file(READ ${GLI_INCLUDE_DIRS}/gli/gli.hpp _CONFIG_FILE)

  # Get version  
  string(REGEX MATCH "define[ \t]+GLI_VERSION_MAJOR[ \t]+([0-9]+)" _MAJOR "${_CONFIG_FILE}")
  set(GLI_VERSION_MAJOR "${CMAKE_MATCH_1}")

  string(REGEX MATCH "define[ \t]+GLI_VERSION_MINOR[ \t]+([0-9]+)" _MINOR "${_CONFIG_FILE}")
  set(GLI_VERSION_MINOR "${CMAKE_MATCH_1}")

  set(GLI_VERSION "${GLI_VERSION_MAJOR}.${GLI_VERSION_MINOR}")
endif()

#===---------------------------------------------------------------------------------------------===
#   Report result 
#====--------------------------------------------------------------------------------------------===
find_package_handle_standard_args(
  gli
  FAIL_MESSAGE "Could NOT find GLI. (Try setting GLI_ROOT in the env)"
  REQUIRED_VARS  
    GLI_ROOT
    GLI_INCLUDE_DIRS
  VERSION_VAR GLI_VERSION
)

if(NOT(GLI_FOUND) AND gli_FIND_REQUIRED EQUAL 1)
  message(FATAL_ERROR "Could NOT find GLI. (Try setting GLI_ROOT in the env)")
endif()

