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
# FindGLSLValidator
# -----------------
#
# This script locates the glslangValidator. This script makes use of the standard find_package 
# arguments ``REQUIRED`` and ``QUIET``. GLSLANGVALIDATOR_FOUND will report if glslangValidator 
# has been found.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# Defines the following variables:
#
#   GLSLANGVALIDATOR_FOUND           - System has glslangValidator
#   GLSLANGVALIDATOR_EXECUTABLE      - The location glslangValidator
#
# Hints
# ^^^^^
#
# You can directly set ``GLSLANGVALIDATOR_EXECUTABLE`` if the script has trouble finding 
# glslangValidator.

include(FindPackageHandleStandardArgs)

find_program(GLSLANGVALIDATOR_EXECUTABLE 
  NAMES glslangValidator
  HINTS ${GLSLANG_ROOT}/bin
)

find_package_handle_standard_args(glslangValidator 
  FOUND_VAR 
    GLSLANGVALIDATOR_FOUND 
  REQUIRED_VARS 
    GLSLANGVALIDATOR_EXECUTABLE
)

mark_as_advanced(GLSLANGVALIDATOR_EXECUTABLE)

