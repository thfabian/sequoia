##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                      S E Q U O I A
##
## This file is distributed under the MIT License (MIT). 
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

## sequoia_include_guard
## ---------------------
##
## Prevent frequently-included CMake files from being re-parsed multiple times.
##
macro(sequoia_include_guard)
  if(DEFINED "__SEQUOIA_INCLUDE_GUARD_${CMAKE_CURRENT_LIST_FILE}")
    return()
  endif(DEFINED "__SEQUOIA_INCLUDE_GUARD_${CMAKE_CURRENT_LIST_FILE}")

  set("__SEQUOIA_INCLUDE_GUARD_${CMAKE_CURRENT_LIST_FILE}" 1)
endmacro()