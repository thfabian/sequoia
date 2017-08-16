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

#.rst:
# sequoia_add_target_clean_all
# ----------------------------
#
# Provide a ``clean-all`` target which clears the CMake cache and all related CMake files and 
# directories. This effectively removes the following files/directories:
#
#    - ``${CMAKE_BINARY_DIR}/CMakeCache.txt``
#    - ``${CMAKE_BINARY_DIR}/CTestTestfile.cmake``
#    - ``${CMAKE_BINARY_DIR}/cmake_install.cmake``
#    - ``${CMAKE_BINARY_DIR}/CMakeFiles``
#
# .. code-block:: cmake
#
#  sequoia_add_target_clean_all([ARGN...])
#
# ``ARGN``
#   Addtional files or directories to remove.
#
function(sequoia_add_target_clean_all)
  sequoia_get_script_dir(script_dir)
  set(sequoia_add_target_clean_all_extra_args ${ARGN})

  set(input_script ${script_dir}/SequoiaAddTargetCleanAll-Script.cmake.in)
  set(output_script ${CMAKE_BINARY_DIR}/sequoia-cmake/cmake/SequoiaAddTargetCleanAll-Script.cmake)

  # Configure the script
  configure_file(${input_script} ${output_script} @ONLY)

  add_custom_target(clean-all
      COMMAND ${CMAKE_MAKE_PROGRAM} clean
      COMMAND ${CMAKE_COMMAND} -P "${output_script}"
      COMMENT "Removing CMake related files"
  )
endfunction()