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
# Findccache
# ----------
#
# .. code-block:: cmake
#
#   find_package(ccache [REQUIRED] [QUIET])
#
# This module locates ccache_. ``CCACHE_FOUND`` will report if ccache has been found.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# Defines the following variables:
#
# ``CCACHE_FOUND``
#   System has ccache.
# ``CCACHE_EXECUTABLE``
#   Path to the ccache executable.
#
# Hints
# ^^^^^
#
# You can directly set ``CCACHE_EXECUTABLE`` if the module has trouble finding ccache.
#
# .. _ccache: https://ccache.samba.org

include(FindPackageHandleStandardArgs)

if(NOT DEFINED CCACHE_EXECUTABLE)
  find_program(CCACHE_EXECUTABLE 
    NAMES ccache
    DOC "Path to ccache executable"
  )
endif()

find_package_handle_standard_args(ccache 
  FOUND_VAR 
    CCACHE_FOUND 
  REQUIRED_VARS 
    CCACHE_EXECUTABLE
)

mark_as_advanced(CCACHE_EXECUTABLE)

