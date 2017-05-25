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
# Findglm
# ---------
#
# This script locates glm. This script makes use of the standard find_package arguments of
# ``VERSION``, ``REQUIRED`` and ``QUIET``. GLM_FOUND will report if an acceptable version of
# glm was found.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# Defines the following variables:
#
#   GLM_FOUND               - System has the glm headers
#   GLM_INCLUDE_DIRS        - The location of GLM headers (i.e the header glm.hpp is in 
#                             ${GLM_INCLUDE_DIRS}/glm/glm.hpp)
#   GLM_VERSION             - Version of glm
#
# Hints
# ^^^^^
#
# Set ``GLM_ROOT`` to a directory that contains a GLM installation, or directly set 
# ``GLM_INCLUDE_DIRS``.

include(FindPackageHandleStandardArgs)

#
# Parse GLM_ROOT
#
set(GLM_ROOT_ENV $ENV{GLM_ROOT})
if(GLM_ROOT_ENV)
  set(GLM_ROOT ${GLM_ROOT_ENV} CACHE PATH "glm install path.")
endif()

if(NOT(GLM_ROOT))
  find_path(GLM_ROOT NAMES glm/glm.hpp HINTS ${GLM_INCLUDE_DIRS})
else()
  get_filename_component(_GLM_ROOT_ABSOLUTE ${GLM_ROOT} ABSOLUTE)
  set(GLM_ROOT ${_GLM_ROOT_ABSOLUTE} CACHE PATH "glm install path.")
endif()

#
# Default version is 0.9
#
if(NOT glm_FIND_VERSION)
  if(NOT glm_FIND_VERSION_MAJOR)
    set(glm_FIND_VERSION_MAJOR 0)
  endif(NOT glm_FIND_VERSION_MAJOR)
  if(NOT glm_FIND_VERSION_MINOR)
    set(glm_FIND_VERSION_MINOR 9)
  endif(NOT glm_FIND_VERSION_MINOR)
  set(glm_FIND_VERSION "${glm_FIND_VERSION_MAJOR}.${glm_FIND_VERSION_MINOR}")
endif(NOT glm_FIND_VERSION)

#===---------------------------------------------------------------------------------------------===
#   Find glm headers
#====--------------------------------------------------------------------------------------------===
if(NOT(GLM_INCLUDE_DIRS))
  find_path(GLM_INCLUDE_DIRS NAMES glm/glm.hpp HINTS ${GLM_ROOT})
endif()

#===---------------------------------------------------------------------------------------------===
#   Read version from glm/detail/setup.hpp
#====--------------------------------------------------------------------------------------------===
if(GLM_INCLUDE_DIRS AND NOT(GLM_VERSION))
  file(READ ${GLM_INCLUDE_DIRS}/glm/detail/setup.hpp _CONFIG_FILE)

  # Get version  
  string(REGEX MATCH "define[ \t]+GLM_VERSION_MAJOR[ \t]+([0-9]+)" _MAJOR "${_CONFIG_FILE}")
  set(GLM_VERSION_MAJOR "${CMAKE_MATCH_1}")

  string(REGEX MATCH "define[ \t]+GLM_VERSION_MINOR[ \t]+([0-9]+)" _MINOR "${_CONFIG_FILE}")
  set(GLM_VERSION_MINOR "${CMAKE_MATCH_1}")

  set(GLM_VERSION "${GLM_VERSION_MAJOR}.${GLM_VERSION_MINOR}")
endif()

#===---------------------------------------------------------------------------------------------===
#   Report result 
#====--------------------------------------------------------------------------------------------===
find_package_handle_standard_args(
  glm
  FAIL_MESSAGE "Could NOT find GLM. (Try setting GLM_ROOT in the env)"
  REQUIRED_VARS  
    GLM_ROOT
    GLM_INCLUDE_DIRS
  VERSION_VAR GLM_VERSION
)

mark_as_advanced(GLM_INCLUDE_DIRS)

if(NOT(GLM_FOUND) AND glm_FIND_REQUIRED EQUAL 1)
  message(FATAL_ERROR "Could NOT find GLM. (Try setting GLM_ROOT in the env)")
endif()

