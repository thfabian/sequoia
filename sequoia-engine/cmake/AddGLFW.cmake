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

find_package(GLFW 3.2.1 REQUIRED)
include_directories(SYSTEM ${GLFW_INCLUDE_DIRS})

sequoia_export_package(
  PACKAGE glfw
  FOUND ${GLFW_FOUND} 
  VERSION_STR "${GLFW_VERSION}" 
  LIBRARIES ${GLFW_LIBRARIES}
)
