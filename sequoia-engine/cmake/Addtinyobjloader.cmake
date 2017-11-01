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

find_package(tinyobjloader NO_MODULE REQUIRED)

get_property(tinyobjloader_INCLUDE_DIRS TARGET tinyobjloader PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
get_property(tinyobjloader_EXTERNAL_LIBRARIES TARGET tinyobjloader PROPERTY INTERFACE_LINK_LIBRARIES)
get_property(tinyobjloader_LIBRARY TARGET tinyobjloader PROPERTY LOCATION)

sequoia_export_package(
  NAME tinyobjloader
  FOUND ${tinyobjloader_FOUND} 
  VERSION ${tinyobjloader_VERSION}
  LIBRARIES ${tinyobjloader_LIBRARY} ${tinyobjloader_EXTERNAL_LIBRARIES}
  INCLUDE_DIRS ${tinyobjloader_INCLUDE_DIRS}
)
