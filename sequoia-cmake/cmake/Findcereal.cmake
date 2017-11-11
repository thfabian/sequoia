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
# Findcereal
# ----------
#
# .. code-block:: cmake
#
#   find_package(cereal 
#     [REQUIRED]             # Fail with error if cereal is not found
#     [QUIET]                # Supress output
#   )
#
# This module locates cereal_. ``CEREAL_FOUND`` will report if an acceptable version of cereal was 
# found.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# Defines the following variables:
#
# ``CEREAL_FOUND``
#  System has the cereal headers.
# ``CEREAL_INCLUDE_DIRS``
#  The location of cereal headers i.e the header ``cereal.hpp`` is in 
#  ``${CEREAL_INCLUDE_DIRS}/cereal/cereal.hpp``.
#
# Hints
# ^^^^^
#
# Set ``CEREAL_ROOT`` to a directory that contains a cereal installation, or directly set 
# ``CEREAL_INCLUDE_DIRS`` if the module has trouble finding cereal.
#
# .. _cereal: https://github.com/USCiLab/cereal
#

include(FindPackageHandleStandardArgs)

#
# Parse CEREAL_ROOT
#
set(CEREAL_ROOT_ENV $ENV{CEREAL_ROOT})
if(CEREAL_ROOT_ENV)
  set(CEREAL_ROOT ${CEREAL_ROOT_ENV} CACHE PATH "cereal install path.")
endif()

if(NOT(CEREAL_ROOT))
  find_path(CEREAL_ROOT NAMES cereal/cereal.hpp HINTS ${CEREAL_INCLUDE_DIRS})
else()
  get_filename_component(_CEREAL_ROOT_ABSOLUTE ${CEREAL_ROOT} ABSOLUTE)
  set(CEREAL_ROOT ${_CEREAL_ROOT_ABSOLUTE} CACHE PATH "cereal install path.")
endif()

#===---------------------------------------------------------------------------------------------===
#   Find cereal headers
#====--------------------------------------------------------------------------------------------===
if(NOT(CEREAL_INCLUDE_DIRS))
  find_path(CEREAL_INCLUDE_DIRS NAMES cereal/cereal.hpp HINTS ${CEREAL_ROOT}/include)
endif()

#===---------------------------------------------------------------------------------------------===
#   Report result 
#====--------------------------------------------------------------------------------------------===
find_package_handle_standard_args(
  cereal
  FAIL_MESSAGE "Could NOT find CEREAL. (Try setting CEREAL_ROOT in the env)"
  REQUIRED_VARS  
    CEREAL_ROOT
    CEREAL_INCLUDE_DIRS
)

if(NOT(CEREAL_FOUND) AND cereal_FIND_REQUIRED EQUAL 1)
  message(FATAL_ERROR "Could NOT find CEREAL. (Try setting CEREAL_ROOT in the env)")
endif()

