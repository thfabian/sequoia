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

set(GL_SOURCE
  GL/GL.h
  GL/GLBuffer.cpp
  GL/GLBuffer.h
  GL/GLExtensionManager.cpp
  GL/GLExtensionManager.h
  GL/GLFragmentData.cpp
  GL/GLFragmentData.h
  GL/GLFwd.h  
  GL/GLIndexBuffer.cpp
  GL/GLIndexBuffer.h
  GL/GLProgram.cpp
  GL/GLProgram.h
  GL/GLProgramManager.cpp
  GL/GLProgramManager.h
  GL/GLPixelFormat.h
  GL/GLPixelFormat.cpp
  GL/GLRenderer.cpp
  GL/GLRenderer.h
  GL/GLRenderSystem.cpp
  GL/GLRenderSystem.h
  GL/GLRenderWindow.cpp
  GL/GLRenderWindow.h
  GL/GLShader.cpp
  GL/GLShader.h
  GL/GLShaderManager.cpp
  GL/GLShaderManager.h
  GL/GLTexture.cpp
  GL/GLTexture.h
  GL/GLTextureManager.cpp
  GL/GLTextureManager.h
  GL/GLVertexAttribute.cpp
  GL/GLVertexAttribute.h
  GL/GLVertexBuffer.cpp
  GL/GLVertexBuffer.h
  GL/GLVertexData.cpp
  GL/GLVertexData.h
  GL/Native.cpp
  GL/Native.h
  GL/NativeGLFW3.cpp
  GL/NativeGLFW3.h
)

set(NULL_SOURCE
  Null/NullFwd.h
  Null/NullIndexBuffer.cpp
  Null/NullIndexBuffer.h
  Null/NullInputSystem.h
  Null/NullProgram.cpp
  Null/NullProgram.h
  Null/NullRenderer.cpp
  Null/NullRenderer.h
  Null/NullRenderSystem.cpp
  Null/NullRenderSystem.h
  Null/NullRenderWindow.cpp
  Null/NullRenderWindow.h
  Null/NullShader.cpp
  Null/NullShader.h
  Null/NullTexture.cpp
  Null/NullTexture.h
  Null/NullVertexBuffer.cpp
  Null/NullVertexBuffer.h
  Null/NullVertexData.cpp
  Null/NullVertexData.h
)

sequoia_add_library(
  NAME SequoiaEngineRender
  SOURCES Buffer.h
          Buffer.cpp
          Camera.cpp
          Camera.h
          DrawCommand.cpp
          DrawCommand.h
          DrawCallContext.h
          DrawScene.h
          Exception.h
          FrameBuffer.cpp
          FrameBuffer.h
          FrameListener.h
          HostBuffer.cpp
          HostBuffer.h
          IndexBuffer.cpp
          IndexBuffer.h
          Input.cpp
          Input.h
          Program.cpp
          Program.h
          RenderCommand.cpp
          RenderCommand.h
          Renderer.cpp
          Renderer.h
          RenderFwd.h
          RenderPass.h
          RenderPipeline.cpp
          RenderPipeline.h
          RenderRessource.cpp
          RenderRessource.h
          RenderServer.cpp
          RenderServer.h
          RenderState.inc
          RenderSystem.cpp
          RenderSystem.h
          RenderSystemObject.h
          RenderTarget.cpp
          RenderTarget.h
          RenderTechnique.h
          RenderBuffer.h
          RenderWindow.h
          Shader.cpp
          Shader.h
          Texture.cpp
          Texture.h
          UniformBlock.h
          UniformStruct.h
          UniformVariable.cpp
          UniformVariable.h
          UniformVariable.inc
          Vertex.h
          VertexAdapter.cpp         
          VertexAdapter.h
          VertexBuffer.cpp
          VertexBuffer.h
          VertexData.cpp
          VertexData.h
          VertexDef.h
          VertexFactory.h
          VertexLayout.cpp
          VertexLayout.h
          VertexLayout.inc          
          ViewFrustum.h
          Viewport.cpp
          Viewport.h
          
          ${GL_SOURCE}
          ${NULL_SOURCE}
  OBJECT
)
