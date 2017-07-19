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

find_package(OpenCV NO_MODULE REQUIRED core highgui imgcodecs imgproc)
include_directories(SYSTEM ${OpenCV_INCLUDE_DIRS})

sequoia_export_package(
  PACKAGE OpenCV
  FOUND ${OPENCV_FOUND} 
  VERSION_STR "${OpenCV_VERSION}" 
  LIBRARIES ${OpenCV_LIBS}
)
