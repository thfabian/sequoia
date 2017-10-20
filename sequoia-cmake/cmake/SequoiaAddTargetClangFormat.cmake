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
include(CMakeParseArguments)

#.rst:
# sequoia_add_target_clang_format
# -------------------------------
#
# Provide a ``format`` target which runs clang-format_ recursively on all files in the provided 
# directories.
#
# .. code-block:: cmake
#
#  sequoia_add_target_clang_format(DIRECTORIES PATTERN)
#
# ``DIRECTORIES``
#   Directories to recursively traverse to find all files with extensions matching ``EXTENSION``.
# ``EXTENSION``
#   Extension to match, separated by ``;``. For example: ``".h;.cpp"``. 
#
# .. _clang-format: https://clang.llvm.org/docs/ClangFormat.html
#
function(sequoia_add_target_clang_format)
  set(options)
  set(one_value_args)
  set(multi_value_args DIRECTORIES EXTENSION)
  cmake_parse_arguments(ARG "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})
  
  if(NOT("${ARG_UNPARSED_ARGUMENTS}" STREQUAL ""))
    message(FATAL_ERROR "sequoia_add_target_clang_format: invalid argument ${ARG_UNPARSED_ARGUMENTS}")
  endif()

  if(NOT(CLANG-FORMAT_EXECUTABLE))
    return()
  endif()

  sequoia_get_script_dir(script_dir)

  # Set configure arguments
  set(CLANG-FORMAT_DIRECTORIES ${ARG_DIRECTORIES})
  set(CLANG-FORMAT_EXTENSION ${ARG_EXTENSION})

  set(input_script 
      ${script_dir}/SequoiaAddTargetClangFormat-Script.cmake.in)
  set(output_script 
      ${CMAKE_BINARY_DIR}/sequoia-cmake/cmake/SequoiaAddTargetClangFormat-Script-Format.cmake)
  
  configure_file(${input_script} ${output_script} @ONLY)
  
  add_custom_target(format
      COMMAND ${CMAKE_COMMAND} -P "${output_script}"
      COMMENT "Running clang-format"
  )
endfunction()