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

# sequoia_make_cmake_script
# -------------------------
#
# Create bash script for re-invoking CMake in the build directory. 
#
#   CMAKE_LISTS_DIR:PATH=<>   - Path to the CMakeLists.txt to invoke
#   BUILD_DIR:PATH=<>         - Directory to install the script
#   ARGN:LIST=<>              - List of arguments used to invoke CMake
#
function(sequoia_make_cmake_script CMAKE_LISTS_DIR BUILD_DIR)
  foreach(arg ${ARGN})
    string(REGEX MATCH "^(.*)=+(.*)$" dummy ${arg})
    if(NOT(CMAKE_MATCH_1) AND NOT(CMAKE_MATCH_2))
      set(script_args "${script_args} ${arg}")
    else()
      # Value of the option has to be quoted for Windows
      set(script_args "${script_args} ${CMAKE_MATCH_1}=\"${CMAKE_MATCH_2}\"")
    endif()
  endforeach()
  
  set(script_args "-G \"${CMAKE_GENERATOR}\" ${script_args}")

  if(WIN32)
   file(WRITE ${BUILD_DIR}/rerun-cmake.bat 
        "\"${CMAKE_COMMAND}\" \"${CMAKE_LISTS_DIR}\" ${script_args}\n")
  else()
   file(WRITE ${BUILD_DIR}/rerun-cmake.sh 
       "#!/usr/bin/env bash\n${CMAKE_COMMAND} ${CMAKE_LISTS_DIR} ${script_args}\n")
  endif()
endfunction()