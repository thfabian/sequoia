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

find_package(assimp NO_MODULE REQUIRED)

find_library(assimp_lib ${ASSIMP_LIBRARIES} HINTS ${ASSIMP_LIBRARY_DIRS})
if(NOT assimp_lib)
  message(FATAL_ERROR "cannot find assimp library in ${ASSIMP_LIBRARY_DIRS}")
endif()

sequoia_export_package(
  NAME assimp
  FOUND ${assimp_FOUND} 
  LIBRARIES ${assimp_lib} 
  VERSION ${assimp_VERSION}
  INCLUDE_DIRS ${ASSIMP_INCLUDE_DIRS}
)
