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
# Findstb
# ---------
#
# This script locates stb. This script makes use of the standard find_package arguments of
# ``VERSION``, ``REQUIRED`` and ``QUIET``. STB_FOUND will report if an acceptable version of
# stb was found.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# Defines the following variables:
#
#   STB_FOUND               - System has the stb headers
#   STB_INCLUDE_DIRS        - The location of STB headers (i.e the header stb.h is in 
#                             ${STB_INCLUDE_DIRS}/stb/stb.h)
#
# Hints
# ^^^^^
#
# Set ``STB_ROOT`` to a directory that contains a STB installation, or directly set 
# ``STB_INCLUDE_DIRS``.

include(FindPackageHandleStandardArgs)

#
# Parse STB_ROOT
#
set(STB_ROOT_ENV $ENV{STB_ROOT})
if(STB_ROOT_ENV)
  set(STB_ROOT ${STB_ROOT_ENV} CACHE PATH "stb install path.")
endif()

if(NOT(STB_ROOT))
  find_path(STB_ROOT NAMES include/stb/stb.h HINTS ${STB_INCLUDE_DIRS})
else()
  get_filename_component(_STB_ROOT_ABSOLUTE ${STB_ROOT} ABSOLUTE)
  set(STB_ROOT ${_STB_ROOT_ABSOLUTE} CACHE PATH "stb install path.")
endif()

#===---------------------------------------------------------------------------------------------===
#   Find stb headers
#====--------------------------------------------------------------------------------------------===
if(NOT(STB_INCLUDE_DIRS))
  find_path(STB_INCLUDE_DIRS NAMES stb/stb.h HINTS ${STB_ROOT}/include)
endif()

#===---------------------------------------------------------------------------------------------===
#   Report result 
#====--------------------------------------------------------------------------------------------===
find_package_handle_standard_args(
  stb
  FAIL_MESSAGE "Could NOT find STB. (Try setting STB_ROOT in the env)"
  REQUIRED_VARS  
    STB_ROOT
    STB_INCLUDE_DIRS
)

mark_as_advanced(STB_INCLUDE_DIRS)

if(NOT(STB_FOUND) AND stb_FIND_REQUIRED EQUAL 1)
  message(FATAL_ERROR "Could NOT find stb. (Try setting STB_ROOT in the env)")
endif()

