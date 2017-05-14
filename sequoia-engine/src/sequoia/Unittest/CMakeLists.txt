##===------------------------------------------------------------------------------*- CMake -*-===##
##                         _____                        _       
##                        / ____|                      (_)      
##                       | (___   ___  __ _ _   _  ___  _  __ _ 
##                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
##                        ____) |  __/ (_| | |_| | (_) | | (_| |
##                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine
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
          RessourcePath.h
          RessourcePath.cpp

          GL/GLEnvironment.h
          GL/GLEnvironment.cpp
          GL/GLRenderTest.h
          GL/GLRenderTest.cpp
  DEPENDS SequoiaDriver
          SequoiaRender
          SequoiaCore
          ${SEQUOIA_GTEST_LIBRARIES}
)
