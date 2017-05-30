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

#ifndef SEQUOIA_RENDER_GL_GLINPUTSYSTEM_H
#define SEQUOIA_RENDER_GL_GLINPUTSYSTEM_H

#include "sequoia/Core/Listenable.h"
#include "sequoia/Render/Export.h"
#include "sequoia/Render/Input.h"

struct GLFWwindow;

namespace sequoia {

namespace render {

class RenderTarget;

/// @brief Handle keyboard and mouse events
/// @ingroup gl
class SEQUOIA_RENDER_API GLInputSystem : public Listenable<KeyboardListener, MouseListener> {
  RenderTarget* target_;

public:
  GLInputSystem(RenderTarget* target);

  /// @brief GLFW callback for the keyboard
  void keyCallback(int key, int action, int mods) noexcept;

  /// @brief GLFW callback for the mouse
  void mouseCallback(int button, int action, int mods) noexcept;
};

} // namespace render

} // namespace sequoia

#endif