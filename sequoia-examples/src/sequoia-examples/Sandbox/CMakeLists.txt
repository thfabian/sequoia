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

set(SEQUOIA_EXAMPLES_SANDBOX_VERSION "0.0.1")

sequoia_configure_file(Config.h.cmake)

sequoia_add_executable( 
  NAME Sandbox
  SOURCES Sandbox.cpp
  DEPENDS ${SEQUOIA_EXAMPLES_LIBRARIES}
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin
)
