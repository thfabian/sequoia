//===--------------------------------------------------------------------------------*- C++ -*-===//
//                         _____                        _
//                        / ____|                      (_)
//                       | (___   ___  __ _ _   _  ___  _  __ _
//                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
//                        ____) |  __/ (_| | |_| | (_) | | (_| |
//                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine
//                                       | |
//                                       |_|
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#ifndef SEQUOIA_GRAPHICS_GL_RENDERSYSTEM_H
#define SEQUOIA_GRAPHICS_GL_RENDERSYSTEM_H

#include "sequoia/Core/Singleton.h"
#include "sequoia/Graphics/RenderSystemImpl.h"
#include <memory>

namespace sequoia {

namespace graphics {

/// @brief OpenGL render-system
/// @ingroup graphics
class SEQUOIA_GRAPHICS_API GLRenderSystem : public RenderSystemImpl {
public:
  /// @brief Initialize GLFW and glbinding
  /// @throws RenderSystemException    Initialization of GLFW failed
  GLRenderSystem();

  /// @brief Terminates GLFW
  ~GLRenderSystem();
};

} // namespace graphics

} // namespace sequoia

#endif
