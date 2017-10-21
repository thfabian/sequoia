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

# sequoia_generate_vertices
# -------------------------
#
# Generate a vertex header and translation unit file. The specification of the vertices is 
# provided via JSON files and parsed using the vertex-gen.py script.
#
#    INPUT_HEADER:STRING=<>     - Template of the header file to generate.
#    OUTPUT_HEADER:STRING=<>    - Generated header file.
#    INPUT_CPP:STRING=<>        - Template of the cpp file to generate.
#    OUTPUT_CPP:STRING=<>       - Generated cpp file.
#    VERTICES:STRING=<>          - Vertices description (*.json)
#
function(sequoia_generate_vertices)
  # Parse arguments
  set(options)
  set(one_value_args INPUT_HEADER INPUT_CPP OUTPUT_HEADER OUTPUT_CPP VERTICES)
  set(multi_value_args)
  cmake_parse_arguments(ARG "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

  if(NOT("${ARG_UNPARSED_ARGUMENTS}" STREQUAL ""))
    message(FATAL_ERROR "invalid argument ${ARG_UNPARSED_ARGUMENTS}")
  endif()

  set(script ${CMAKE_SOURCE_DIR}/scripts/vertex-gen/vertex-gen.py)
  set(script_path "\"${script}\"")
  set(script_args --vertices="${ARG_VERTICES}"
                  --input-header="${ARG_INPUT_HEADER}"
                  --input-cpp="${ARG_INPUT_CPP}"
                  --output-header="${ARG_OUTPUT_HEADER}" 
                  --output-cpp="${ARG_OUTPUT_CPP}")
  string(REPLACE "${CMAKE_BINARY_DIR}/" "" vertex_header "${ARG_OUTPUT_HEADER}")
  
  # Run vertex-gen.py
  add_custom_command(
    OUTPUT ${ARG_OUTPUT_HEADER} ${ARG_OUTPUT_CPP}
    COMMAND ${PYTHON_EXECUTABLE} ${script_path} ${script_args}
    DEPENDS ${ARG_INPUT_HEADER} ${ARG_INPUT_CPP} ${ARG_VERTICES} ${script}
    COMMENT "Generating vertex description ${vertex_header}"
  )
endfunction()
