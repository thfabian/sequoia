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

include(SequoiaIncludeGuard)
sequoia_include_guard()

## .rst
## sequoia_check_in_source_build
## -----------------------------
##
## Abort with a meaningful error message if ``CMAKE_BINARY_DIR`` matches ``CMAKE_SOURCE_DIR``.
##
function(sequoia_check_in_source_build)
  if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR AND NOT MSVC_IDE)
    message(FATAL_ERROR "
  ### In-source builds are not allowed.
  ### Please create a build directory and run cmake from there, 
  ### passing the path to this source directory as the last argument.
  ### This process created the file `CMakeCache.txt' and the 
  ### directory `CMakeFiles'. Please delete them!
")
  endif()
endfunction()