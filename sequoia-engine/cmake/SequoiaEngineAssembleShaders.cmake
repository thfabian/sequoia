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

# sequoia_engine_assemble_shaders
# -------------------------------
#
# Assemble all shader ``SOURCES`` and configure the ``HEADER`` by creating ``TARGET``.
#
#    TARGET:STRING=<>   - Name of the target.
#    HEADER:STRING=<>   - Input header to configure.
#    SOURCES:LIST=<>    - List of shader source files. 
#
function(sequoia_engine_assemble_shaders TARGET HEADER SOURCES)
  set(header "${HEADER}")
  set(source_dir "${CMAKE_CURRENT_SOURCE_DIR}")
  set(binary_dir "${CMAKE_CURRENT_BINARY_DIR}")
  set(sources "${SOURCES}")

  set(input "${CMAKE_SOURCE_DIR}/cmake/scripts/SequoiaEngineAssembleShaders-Script.cmake.in")
  set(output "${CMAKE_BINARY_DIR}/cmake/scripts/SequoiaEngineAssembleShaders-Script.cmake")
  configure_file("${input}" "${output}" @ONLY)

  set(header_input "${source_dir}/${header}")
  set(header_output "${binary_dir}/${header}")
  string(REGEX REPLACE ".cmake" "" header_output ${header_output})

  add_custom_command(
    OUTPUT ${header_output}
    COMMAND ${CMAKE_COMMAND} -P ${output}
    DEPENDS 
      ${header_input}
      ${SOURCES}
    COMMENT "Configuring ${header}"
    VERBATIM
  )
  add_custom_target(${TARGET} DEPENDS ${header_output})
endfunction()
