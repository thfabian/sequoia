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

find_package(spdlog NO_MODULE REQUIRED)

get_property(spdlog_INCLUDE_DIRS TARGET spdlog::spdlog PROPERTY INTERFACE_INCLUDE_DIRECTORIES)

sequoia_export_package(
  NAME spdlog
  FOUND ${spdlog_FOUND} 
  VERSION ${spdlog_VERSION}
  INCLUDE_DIRS ${spdlog_INCLUDE_DIRS}
  DEFINITIONS -DSPDLOG_FMT_EXTERNAL 
)