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

# .rst
# sequoia_include_guard
# ---------------------
#
# Prevent frequently-included CMake files from being re-parsed multiple times.
#
macro(sequoia_include_guard)
  if(DEFINED "__SEQUOIA_INCLUDE_GUARD_${CMAKE_CURRENT_LIST_FILE}")
    return()
  endif(DEFINED "__SEQUOIA_INCLUDE_GUARD_${CMAKE_CURRENT_LIST_FILE}")

  set("__SEQUOIA_INCLUDE_GUARD_${CMAKE_CURRENT_LIST_FILE}" 1)
endmacro()
