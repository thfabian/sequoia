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

find_package(FreeImage REQUIRED)
include_directories(SYSTEM ${FREEIMAGE_INCLUDE_DIRS})

# We use the static library of FreeImage
add_definitions(-DFREEIMAGE_LIB)

sequoia_export_package(
  PACKAGE FreeImage
  FOUND ${FREEIMAGE_FOUND} 
  VERSION_STR "${FREEIMAGE_VERSION}" 
  LIBRARIES ${FREEIMAGE_LIBRARIES}
)
