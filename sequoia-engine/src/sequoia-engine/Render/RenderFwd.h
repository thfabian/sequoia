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

#ifndef SEQUOIA_ENGINE_RENDER_RENDERFWD_H
#define SEQUOIA_ENGINE_RENDER_RENDERFWD_H

#include "sequoia-engine/Render/RenderSystemObject.h"

namespace sequoia {

namespace render {

class Camera;
class DrawCommand;
class DrawCommandList;
class FrameBuffer;
class GlobalRenderState;
class Program;
class RenderStateCache;
class RenderSystem;
class RenderTarget;
class RenderWindow;
class Shader;
class Texture;
class VertexArrayObject;
class VertexData;
class VertexVistor;
class ViewFrustum;
class Viewport;
class RenderCommand;
class UniformVariable;
struct RenderState;
struct TextureParameter;
struct VertexLayout2;

} // namespace render

} // namespace sequoia

#endif
