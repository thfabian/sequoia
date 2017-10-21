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

# sequoia_generate_options
# ------------------------
#
# Generate an the option header and include file. The specification of the options is provided via
# JSON files and parsed using the opt-gen.py script.
#
#    INPUT_HEADER:STRING=<>     - Template of the header file to generate.
#    OUTPUT_HEADER:STRING=<>    - Generated header file.
#    INPUT_INCLUDE:STRING=<>    - Template of the include file to generate.
#    OUTPUT_INCLUDE:STRING=<>   - Generated include file.
#    OPTIONS:STRING=<>          - List input JSON files containing the option specification.
#
function(sequoia_generate_options)
  # Parse arguments
  set(options)
  set(one_value_args INPUT_HEADER INPUT_INCLUDE OUTPUT_HEADER OUTPUT_INCLUDE)
  set(multi_value_args OPTIONS)
  cmake_parse_arguments(ARG "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

  if(NOT("${ARG_UNPARSED_ARGUMENTS}" STREQUAL ""))
    message(FATAL_ERROR "invalid argument ${ARG_UNPARSED_ARGUMENTS}")
  endif()

  set(script ${CMAKE_SOURCE_DIR}/scripts/opt-gen/opt-gen.py)
  set(script_path "\"${script}\"")
  set(script_args --input-header="${ARG_INPUT_HEADER}"
                  --input-include="${ARG_INPUT_INCLUDE}"
                  --output-header="${ARG_OUTPUT_HEADER}" 
                  --output-include="${ARG_OUTPUT_INCLUDE}")
  foreach(opt ${ARG_OPTIONS})
    list(APPEND script_args "\"${opt}\"")
  endforeach()

  string(REPLACE "${CMAKE_BINARY_DIR}/" "" header "${ARG_OUTPUT_HEADER}")
  
  # Run opt-gen.py
  add_custom_command(
    OUTPUT ${ARG_OUTPUT_HEADER} ${ARG_OUTPUT_INCLUDE}
    COMMAND ${PYTHON_EXECUTABLE} ${script_path} ${script_args}
    DEPENDS ${ARG_INPUT_HEADER} ${ARG_INPUT_INCLUDE} ${ARG_OPTIONS} ${script}
    COMMENT "Generating options header ${header}"
  )
endfunction()