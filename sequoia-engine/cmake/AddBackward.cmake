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

find_package(Backward NO_MODULE REQUIRED)

foreach(definition ${BACKWARD_DEFINITIONS})
  list(APPEND backward_definitions -D${definition})
endforeach()

get_property(Backward_INCLUDE_DIRS TARGET Backward::Backward PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
get_property(Backward_LIBRARIES TARGET Backward::Backward PROPERTY INTERFACE_LINK_LIBRARIES)

sequoia_export_package(
  NAME backward 
  FOUND ${Backward_FOUND} 
  LIBRARIES ${Backward_LIBRARIES}
  INCLUDE_DIRS ${Backward_INCLUDE_DIRS}
  DEFINITIONS ${backward_definitions}
)
