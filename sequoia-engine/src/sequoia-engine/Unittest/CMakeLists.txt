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

sequoia_configure_file(Config.h.cmake)

set(GL_SOURCE
  GL/GLTestEnvironment.h
  GL/GLTestEnvironment.cpp
  GL/GLBenchmarkEnvironment.h
  GL/GLBenchmarkEnvironment.cpp
  GL/GLRenderSetup.h
  GL/GLRenderSetup.cpp
)

sequoia_add_library(
  NAME SequoiaEngineUnittest
  SOURCES BenchmarkEnvironment.h
          BenchmarkEnvironment.cpp
          BenchmarkMain.h
          Fixture.h
          GameSetup.h
          GameSetup.cpp
          Test.h
          Test.cpp
          TestEnvironment.h
          TestEnvironment.cpp
          TestFile.h
          TestFile.cpp

          ${GL_SOURCE}
          
  DEPENDS SequoiaEngineRenderObjects
  ARCHIVE
)
