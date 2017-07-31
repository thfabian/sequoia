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

sequoia_export_package(
  PACKAGE backward 
  FOUND ${Backward_FOUND} 
  LIBRARIES ${BACKWARD_LIBRARIES}
  INCLUDE_DIRS ${BACKWARD_INCLUDE_DIRS}
  DEFINITIONS ${backward_definitions}
)
