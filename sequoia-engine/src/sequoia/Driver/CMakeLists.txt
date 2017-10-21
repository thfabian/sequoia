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

sequoia_add_library(
  NAME SequoiaDriver
  SOURCES Driver.cpp
          Driver.h
          CommandLine.h
          CommandLine.cpp
          ConsoleLogger.h
          ConsoleLogger.cpp
          Win32Console.h
          Win32Console.cpp
  DEPENDS SequoiaCoreObjects
  OBJECT
)
