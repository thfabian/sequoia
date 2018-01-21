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
)

set(NULL_SOURCE
  Null/NullTestEnvironment.h
  Null/NullTestEnvironment.cpp
)

if(SEQUOIA_HAS_D3D12)
  set(D3D12_SOURCE
    D3D12/D3D12TestEnvironment.h
    D3D12/D3D12TestEnvironment.cpp
  )
endif()

sequoia_add_library(
  NAME SequoiaEngineUnittest
  SOURCES BenchmarkEnvironment.h
          BenchmarkEnvironment.cpp
          BenchmarkMain.h
          Exception.h
          Fixture.h
          GameSetup.cpp
          GameSetup.h
          RenderSetup.cpp
          RenderSetup.h
          Test.cpp
          Test.h
          TestEnvironment.cpp
          TestEnvironment.h
          TestFile.cpp
          TestFile.h

          ${GL_SOURCE}
          ${NULL_SOURCE}
          ${D3D12_SOURCE}
  ARCHIVE
)
