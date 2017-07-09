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
# FindCppcheck
# ------------
#
# This script locates Cppcheck. This script makes use of the standard find_package arguments 
# ``REQUIRED`` and ``QUIET``. CPPCHECK_FOUND will report if cppcheck has been found.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# Defines the following variables:
#
#   CPPCHECK_FOUND           - System has cppcheck
#   CPPCHECK_EXECUTABLE      - The location cppcheck
#
# Hints
# ^^^^^
#
# You can directly set ``CPPCHECK_EXECUTABLE`` if the script has trouble finding Cppcheck.

include(FindPackageHandleStandardArgs)

if(NOT DEFINED CPPCHECK_EXECUTABLE)
  find_program(CPPCHECK_EXECUTABLE NAMES cppcheck)
endif()

find_package_handle_standard_args(Cppcheck 
  FOUND_VAR 
    CPPCHECK_FOUND 
  REQUIRED_VARS 
    CPPCHECK_EXECUTABLE
)

mark_as_advanced(CPPCHECK_EXECUTABLE)

