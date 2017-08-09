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

include(SequoiaGetScriptDir)

# .rst
# sequoia_add_target_clean_all
# ----------------------------
#
# Provide a ``clean-all`` target which clears the CMake Cache and all related CMake files and 
# directories. This effectively removes the following files in the ``CMAKE_BINARY_DIR``:
#
#    CMakeCache.txt
#    CTestTestfile.cmake
#    cmake_install.cmake
#    CMakeFiles
#
# To provide additional cleanup files or directories:
#
#    ARGN:STRING=<>      - Addtional files or directories (relative to ``CMAKE_BINARY_DIR``) to 
#                          remove.
#
function(sequoia_add_target_clean_all)
  sequoia_get_script_dir(script_dir)
  add_custom_target(clean-all
      COMMAND ${CMAKE_MAKE_PROGRAM} clean
      COMMAND ${CMAKE_COMMAND} -P "${script_dir}/SequoiaAddTargetCleanAll-Script.cmake"
      COMMENT "Removing CMake related files"
 )
endfunction()