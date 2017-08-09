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

get_filename_component(__sequoia_cmake_script_dir__ ${CMAKE_CURRENT_LIST_FILE} PATH)

# .rst
# sequoia_get_script_dir
# ----------------------
#
# Get the directory of the scripts located ``sequoia-cmake/scripts``.
# This function is for interal use only.
#
#    SCRIPT_DIR_VAR:STRING=<>      - Variable will be set to the script directory.
#
function(sequoia_get_script_dir SCRIPT_DIR_VAR)
  set(${SCRIPT_DIR_VAR} "${__sequoia_cmake_script_dir__}/scripts" PARENT_SCOPE)
endfunction()
