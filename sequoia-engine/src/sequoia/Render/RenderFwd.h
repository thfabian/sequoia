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

#ifndef SEQUOIA_RENDER_RENDERFWD_H
#define SEQUOIA_RENDER_RENDERFWD_H

namespace sequoia {

namespace render {

class Camera;
class Program;
class DrawCommand;
class DrawCommandList;
struct RenderState;
class RenderStateCache;
class RenderSystem;
class RenderTarget;
class RenderWindow;
class Shader;
class VertexArrayObject;
struct VertexLayout;
struct Vertex2DLayout;
struct Vertex3DLayout;
class VertexVistor;
class ViewFrustum;
class Viewport;

} // namespace render

} // namespace sequoia

#endif
