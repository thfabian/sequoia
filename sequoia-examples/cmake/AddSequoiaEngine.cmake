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

find_package(SequoiaEngine NO_MODULE REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SequoiaEngine_COMPILE_DEFINITIONS}")
sequoia_export_package(
  NAME SequoiaEngine
  FOUND ${SequoiaEngine_FOUND} 
  VERSION ${SequoiaEngine_VERSION} 
  LIBRARIES sequoia::SequoiaEngineStatic
  INCLUDE_DIRS ${SequoiaEngine_INCLUDE_DIRS}
  DEFINITIONS ${SequoiaEngine_DEFINITIONS}
)
