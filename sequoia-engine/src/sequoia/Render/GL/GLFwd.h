
//===--------------------------------------------------------------------------------*- C++ -*-===//
//                         _____                        _
//                        / ____|                      (_)
//                       | (___   ___  __ _ _   _  ___  _  __ _
//                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
//                        ____) |  __/ (_| | |_| | (_) | | (_| |
//                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine (2016-2017)
//                                       | |
//                                       |_|
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#ifndef SEQUOIA_RENDER_GL_GLFWD_H
#define SEQUOIA_RENDER_GL_GLFWD_H

#include <glbinding/gl/enum.h>
#include <glbinding/gl/extension.h>
#include <glbinding/gl/types.h>

using namespace gl;

namespace sequoia {

namespace render {

class GLBuffer;
class GLExtensionManager;
struct GLFragmentData;
class GLFrameBufferObject;
class GLInputSystem;
class GLProgram;
class GLProgramManager;
class GLIndexBuffer;
class GLRenderer;
class GLRenderSystem;
class GLRenderWindow;
class GLShader;
class GLShaderManager;
class GLStateCacheManager;
class GLTexture;
class GLTextureManager;
class GLVertexArrayObject;
struct GLVertexAttribute;

} // namespace render

} // namespace sequoia

#endif
