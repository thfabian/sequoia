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

find_package(glfw3 3.2.1 NO_MODULE REQUIRED)

get_property(glfw3_INCLUDE_DIRS TARGET glfw PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
include_directories(SYSTEM ${glfw3_INCLUDE_DIRS})

sequoia_export_package(
  PACKAGE glfw3
  FOUND ${glfw3_FOUND} 
  VERSION_STR "${glfw3_VERSION}" 
  LIBRARIES "glfw"
)

