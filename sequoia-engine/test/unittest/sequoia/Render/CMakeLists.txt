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

add_subdirectory(GL)

sequoia_add_test(
  NAME SequoiaRenderTest
  SOURCES TestCamera.cpp
          TestMain.cpp
          TestInput.cpp
          TestRenderState.cpp
          TestTexture.cpp
          TestUniformVariable.cpp
          TestVertex.cpp
          TestVertexData.cpp
  DEPENDS Sequoia
          ${SEQUOIA_EXTERNAL_LIBRARIES}
)
