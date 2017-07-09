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

find_package(Cppcheck)

sequoia_export_package(
  PACKAGE Cppcheck
  FOUND ${CPPCHECK_FOUND} 
)

if(CPPCHECK_FOUND)
  set(SEQUOIA_HAS_CPPCHECK 1)
else()
  set(SEQUOIA_HAS_CPPCHECK 0)
endif()

