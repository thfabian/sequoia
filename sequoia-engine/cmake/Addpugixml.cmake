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

find_package(pugixml NO_MODULE REQUIRED)

get_property(pugixml_INCLUDE_DIRS TARGET pugixml PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
get_property(pugixml_LIBRARY TARGET pugixml PROPERTY LOCATION)

sequoia_export_package(
  NAME pugixml
  FOUND ${pugixml_FOUND} 
  LIBRARIES ${pugixml_LIBRARY}
  VERSION ${pugixml_VERSION}
  INCLUDE_DIRS ${pugixml_INCLUDE_DIRS}
)