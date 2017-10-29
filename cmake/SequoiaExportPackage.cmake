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

include(CMakeParseArguments)
include(SequoiaAppendAndExportVariable)

# sequoia_export_package
# ----------------------
#
# Export the <PACKAGE> by appending <CMAKE_ARGS> to SEQUOIA_<PACKAGE>_CMAKE_ARGS.
#
#    PACKAGE:STRING=<>      - Name of the package
#    CMAKE_ARGS:LIST=<>     - Arguments passed to find_package.
#
function(sequoia_export_package)
  set(options)
  set(one_value_args PACKAGE)
  set(multi_value_args CMAKE_ARGS)
  cmake_parse_arguments(ARG "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

  if(NOT("${ARG_UNPARSED_ARGUMENTS}" STREQUAL ""))
    message(FATAL_ERROR "invalid argument ${ARG_UNPARSED_ARGUMENTS}")
  endif()

  string(TOUPPER ${ARG_PACKAGE} package_upper)
  sequoia_append_and_export_variable("SEQUOIA_${package_upper}_CMAKE_ARGS" ${ARG_CMAKE_ARGS})
endfunction()
