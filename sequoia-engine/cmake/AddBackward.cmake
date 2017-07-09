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

find_package(Backward REQUIRED)
include_directories(SYSTEM ${BACKWARD_INCLUDE_DIRS})

set(BACKWARD_LIBRARIES)
if(BACKWARD_HAS_DW)
  include_directories(SYSTEM ${BACKWARD_DW_INCLUDE_DIRS})
  set(BACKWARD_LIBRARIES ${BACKWARD_DW_LIBRARIES})
  add_definitions(-DSEQUOIA_BACKWARD_HAS_DW=1)
elseif(BACKWARD_HAS_BFD)
  include_directories(SYSTEM ${BACKWARD_BFD_INCLUDE_DIRS})
  set(BACKWARD_LIBRARIES ${BACKWARD_BFD_LIBRARIES})
  add_definitions(-DSEQUOIA_BACKWARD_HAS_BFD=1)
endif()

sequoia_export_package(
  PACKAGE Backward 
  FOUND ${BACKWARD_FOUND} 
  LIBRARIES ${BACKWARD_LIBRARIES}
)

