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

sequoia_add_test(
  NAME SequoiaRenderGLTest
  SOURCES TestMain.cpp
          TestGLProgram.cpp
          TestGLRenderWindow.cpp
          TestGLShader.cpp
          TestGLVertexArrayObject.cpp
  DEPENDS SequoiaUnittest
          SequoiaRender
          SequoiaDriver
          SequoiaCore
          ${SEQUOIA_UNITTEST_EXTERNAL_LIBRARIES}
)

