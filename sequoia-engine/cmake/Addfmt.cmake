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

find_package(fmt NO_MODULE REQUIRED)

get_property(fmt_INCLUDE_DIRS TARGET fmt::fmt PROPERTY INTERFACE_INCLUDE_DIRECTORIES)

sequoia_export_package(
  NAME fmt
  FOUND ${fmt_FOUND} 
  LIBRARIES "fmt::fmt"
  VERSION ${fmt_VERSION}
  INCLUDE_DIRS ${fmt_INCLUDE_DIRS}
)
