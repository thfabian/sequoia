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
# Assemble all shader sources and configure the ``HEADER``.
#
macro(sequoia_engine_assemble_shaders HEADER SOURCES)
  set(SEQUOIA_ENGINE_SHADER_SOURCES)
  foreach(source ${SOURCES})
    get_filename_component(path ${source} ABSOLUTE)
    get_filename_component(name ${source} NAME)
    file(READ ${path} content)
    set(SEQUOIA_ENGINE_SHADER_SOURCES 
        "${SEQUOIA_ENGINE_SHADER_SOURCES}\n// ${source}\nSHADER(\"${name}\", R\"(${content})\")\n")
  endforeach()
  sequoia_configure_file(${HEADER})
endmacro()
