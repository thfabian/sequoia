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

#ifndef SEQUOIA_RENDER_GL_GLRENDERER_H
#define SEQUOIA_RENDER_GL_GLRENDERER_H

#include "sequoia/Render/Export.h"

namespace sequoia {

namespace render {

class GLRenderWindow;

/// @brief OpenGL based renderer
///
/// A rendered performs the actual rendering to the associated target. Further, the renderer keeps
/// tabs on the OpenGL state machine and is it the only one who is allowed to perform OpenGL API
/// calls
/// @ingroup gl
class GLRenderer {
  GLRenderWindow* target_;

public:
  /// @brief Initialize the OpenGL context and bind it to the calling thread
  GLRenderer(GLRenderWindow* target);
  
  /// @brief Release the OpenGL context
  ~GLRenderer();

  /// @brief Render the given target
  void render();
};

} // namespace render

} // namespace sequoia

#endif