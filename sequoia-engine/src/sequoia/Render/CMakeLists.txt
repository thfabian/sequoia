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

sequoia_add_library(
  NAME SequoiaRender
  SOURCES Camera.h
          Camera.cpp
          DrawCommand.h
          DrawCommand.cpp
          DrawCommandList.h
          DrawCommandList.cpp
          Export.h
          Exception.h
          Input.h
          Program.h
          Program.cpp
          RenderFwd.h
          RenderSystem.h
          RenderSystem.cpp
          RenderSystemObject.h
          RenderTarget.h
          RenderTarget.cpp
          RenderWindow.h
          Shader.h
          Shader.cpp
          Vertex.h
          Vertex.cpp
          VertexArrayObject.h
          VertexArrayObject.cpp
          ViewFrustum.h
          Viewport.h
          Viewport.cpp
          
          GL/GL.h
          GL/GLInputSystem.h
          GL/GLInputSystem.cpp
          GL/GLProgram.h
          GL/GLProgram.cpp
          GL/GLProgramManager.h
          GL/GLProgramManager.cpp
          GL/GLRenderer.h
          GL/GLRenderer.cpp
          GL/GLRenderSystem.h
          GL/GLRenderSystem.cpp
          GL/GLRenderWindow.h
          GL/GLRenderWindow.cpp
          GL/GLShader.h
          GL/GLShader.cpp
          GL/GLShaderManager.h
          GL/GLShaderManager.cpp
          GL/GLVertexArrayObject.h
          GL/GLVertexArrayObject.cpp
          GL/GLVertexAttribute.h
          GL/GLVertexAttribute.cpp
          
  DEPENDS SequoiaCore
          ${SEQUOIA_EXTERNAL_LIBRARIES}
)
