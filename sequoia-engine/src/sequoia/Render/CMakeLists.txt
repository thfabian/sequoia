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

sequoia_add_objects(
  NAME SequoiaRender
  SOURCES Buffer.h
          Buffer.cpp
          Camera.h
          Camera.cpp
          DrawCommand.h
          DrawCommand.cpp
          DrawCommandList.h
          DrawCommandList.cpp
          Exception.h
          FrameListener.h
          FrameBufferObject.h
          FrameBufferObject.cpp
          HostBuffer.h
          HostBuffer.cpp
          IndexBuffer.h
          IndexBuffer.cpp
          Input.h
          Input.cpp
          Program.h
          Program.cpp
          RenderFwd.h
          RenderRessource.h
          RenderRessource.cpp
          RenderServer.h
          RenderServer.cpp
          RenderState.h
          RenderState.cpp
          RenderState.inc
          RenderSystem.h
          RenderSystem.cpp
          RenderSystemObject.h
          RenderTarget.h
          RenderTarget.cpp
          RenderWindow.h
          Shader.h
          Shader.cpp
          Texture.h
          Texture.cpp
          UniformVariable.h
          UniformVariable.cpp
          UniformVariable.inc
          Vertex.h
          Vertex.cpp
          VertexArrayObject.h
          VertexArrayObject.cpp
          VertexData.h
          VertexData.cpp
          VertexVisitor.h
          VertexVisitor.cpp
          ViewFrustum.h
          Viewport.h
          Viewport.cpp
          
          GL/GL.h
          GL/GLBuffer.h
          GL/GLBuffer.cpp          
          GL/GLExtensionManager.h
          GL/GLExtensionManager.cpp
          GL/GLFragmentData.h
          GL/GLFragmentData.cpp
          GL/GLFwd.h  
          GL/GLFrameBufferObject.h
          GL/GLFrameBufferObject.cpp
          GL/GLProgram.h
          GL/GLProgram.cpp
          GL/GLProgramManager.h
          GL/GLProgramManager.cpp
          GL/GLIndexBuffer.h
          GL/GLIndexBuffer.cpp
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
          GL/GLStateCacheManager.h
          GL/GLStateCacheManager.cpp
          GL/GLTexture.h
          GL/GLTexture.cpp
          GL/GLTextureManager.h
          GL/GLTextureManager.cpp
          GL/GLVertexArrayObject.h
          GL/GLVertexArrayObject.cpp
          GL/GLVertexAttribute.h
          GL/GLVertexAttribute.cpp
          GL/Native.h
          GL/Native.cpp          
          GL/NativeGLFW3.h
          GL/NativeGLFW3.cpp

  DEPENDS SequoiaCore
)
