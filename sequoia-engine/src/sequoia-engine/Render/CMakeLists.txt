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

include(SequoiaGenerateVertices)

# Generate the Vertex.h and Vertex.cpp
set(vertex_input_header ${CMAKE_CURRENT_SOURCE_DIR}/Vertex.h.in)
set(vertex_input_cpp ${CMAKE_CURRENT_SOURCE_DIR}/Vertex.cpp.in)
set(vertex_output_header ${CMAKE_CURRENT_BINARY_DIR}/Vertex.h)
set(vertex_output_cpp ${CMAKE_CURRENT_BINARY_DIR}/Vertex.cpp)

sequoia_generate_vertices(
  VERTICES ${CMAKE_CURRENT_SOURCE_DIR}/Vertex.json
  INPUT_HEADER ${vertex_input_header}
  INPUT_CPP ${vertex_input_cpp}
  OUTPUT_HEADER ${vertex_output_header}
  OUTPUT_CPP ${vertex_output_cpp}
)

set(GL_SOURCE
  GL/GL.h
  GL/GLBuffer.cpp          
  GL/GLBuffer.h
  GL/GLExtensionManager.cpp
  GL/GLExtensionManager.h
  GL/GLFragmentData.cpp
  GL/GLFragmentData.h
  GL/GLFrameBufferObject.cpp
  GL/GLFrameBufferObject.h
  GL/GLFwd.h  
  GL/GLIndexBuffer.cpp
  GL/GLIndexBuffer.h
  GL/GLProgram.cpp
  GL/GLProgram.h
  GL/GLProgramManager.cpp
  GL/GLProgramManager.h
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
  GL/GLStateCacheManager.cpp
  GL/GLStateCacheManager.h
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
          DrawCommandList.cpp
          DrawCommandList.h
          Exception.h
          FrameBuffer.cpp
          FrameBuffer.h
          FrameListener.h
          GlobalRenderState.cpp
          GlobalRenderState.h
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
          RenderFwd.h
          RenderRessource.cpp
          RenderRessource.h
          RenderServer.cpp
          RenderServer.h
          RenderState.cpp
          RenderState.h
          RenderState.inc
          RenderSystem.cpp
          RenderSystem.h
          RenderSystemObject.h
          RenderTarget.cpp
          RenderTarget.h
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
          Vertex2.h
          VertexAdapter.h
          VertexBuffer.cpp
          VertexBuffer.h
          VertexDef.h
          VertexLayout.cpp
          VertexLayout.h
          VertexData.cpp
          VertexData.h
          VertexVisitor.cpp
          VertexVisitor.h
          ViewFrustum.h
          Viewport.cpp
          Viewport.h

          Vertex.json
          ${vertex_output_header}
          ${vertex_output_cpp}
          ${vertex_input_header}
          ${vertex_input_cpp}
          
          ${GL_SOURCE}
          ${NULL_SOURCE}
  OBJECT
)
