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

#ifndef SEQUOIA_RENDER_GL_GLINPUTSYSTEM_H
#define SEQUOIA_RENDER_GL_GLINPUTSYSTEM_H

#include "sequoia/Core/Export.h"
#include "sequoia/Core/Listenable.h"
#include "sequoia/Render/Input.h"

struct GLFWwindow;

namespace sequoia {

namespace render {

class GLRenderWindow;

/// @brief Handle keyboard and mouse events
/// @ingroup gl
class SEQUOIA_API GLInputSystem : public Listenable<KeyboardListener, MouseListener> {
  GLRenderWindow* window_;

  /// Previous mouse positions
  int prevPosX_, prevPosY_;

  /// Ignore the next mouse position event (by default the first mouse event is dropped as on Linux
  /// this sometimes has some wired coordiantes especially if launched on a secondary monitor)
  bool ignoreNextMousePosEvent_;

public:
  GLInputSystem(GLRenderWindow* target, bool centerCursor);

  /// @brief GLFW callback for the keyboard
  void keyCallback(int key, int action, int mods) noexcept;

  /// @brief GLFW callback for the mouse button
  void mouseButtonCallback(int button, int action, int mods) noexcept;

  /// @brief GLFW callback for the mouse position
  void mousePositionCallback(int xpos, int ypos) noexcept;

  /// @brief Center the cursor
  void centerCursor();

  /// @brief Set mouse position to `(xpos, ypos)`
  void setCursorPosition(int xpos, int ypos);
};

} // namespace render

} // namespace sequoia

#endif
