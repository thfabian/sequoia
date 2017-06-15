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

# Generate Config.h
sequoia_configure_file(Config.h.cmake)

sequoia_add_library(
  NAME SequoiaUnittest
  SOURCES Environment.h
          Environment.cpp
          Export.h
          GameTest.h
          GameTest.cpp
          TestFile.h
          TestFile.cpp
          Test.h
          Test.cpp
          
          GL/GLEnvironment.h
          GL/GLEnvironment.cpp
          GL/GLRenderTest.h
          GL/GLRenderTest.cpp
  DEPENDS SequoiaDriver
          SequoiaRender
          SequoiaCore
          ${SEQUOIA_GTEST_LIBRARIES}
)
