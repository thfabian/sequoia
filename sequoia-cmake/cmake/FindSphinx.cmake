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
# FindSphinx
# ----------
#
# This script locates Sphinx. This script makes use of the standard find_package arguments 
# ``REQUIRED`` and ``QUIET``. SPHINX_FOUND will report if Sphinx has been found.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# Defines the following variables:
#
#   SPHINX_FOUND           - System has Sphinx
#   SPHINX_EXECUTABLE      - The location of Sphinx binary
#
# Hints
# ^^^^^
#
# You can directly set ``SPHINX_EXECUTABLE`` if the script has trouble finding SPHINX.

include(FindPackageHandleStandardArgs)

if(NOT DEFINED SPHINX_EXECUTABLE)
  find_program(SPHINX_EXECUTABLE 
    NAMES sphinx-build sphinx-build2 
    DOC "Path to sphinx-build executable"
  )
endif()

find_package_handle_standard_args(sphinx 
  FOUND_VAR 
    SPHINX_FOUND 
  REQUIRED_VARS
    SPHINX_EXECUTABLE
)

mark_as_advanced(SPHINX_EXECUTABLE)