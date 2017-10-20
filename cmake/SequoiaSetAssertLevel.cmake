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

# sequoia_set_assert_level
# ------------------------
#
# Adjust the CMake options (CMAKE_CXX_FLAGS and CMAKE_C_FLAGS) to enable asserts.
#
#    OUT_VAR:STRING=<>      - Variable to append the options
#
macro(sequoia_set_assert_level OUT_VAR)
  if(WIN32)
    set(ndebug_flag "/UNDEBUG")
  else()
    set(ndebug_flag "-UNDEBUG")
  endif()
  list(APPEND "${OUT_VAR}" "-DCMAKE_CXX_FLAGS:STRING=${ndebug_flag}" 
                           "-DCMAKE_C_FLAGS:STRING=${ndebug_flag}")
endmacro()
