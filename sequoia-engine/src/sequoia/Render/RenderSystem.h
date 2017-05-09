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

#ifndef SEQUOIA_RENDER_RENDERSYSTEM_H
#define SEQUOIA_RENDER_RENDERSYSTEM_H

#include "sequoia/Core/Platform.h"
#include "sequoia/Render/Export.h"
#include "sequoia/Render/RenderFwd.h"
#include <memory>
#include <string>

namespace sequoia {

namespace render {

/// @brief Currently active render-system
///
/// The render-system takes care of window and context creation as well as setting up the IO
/// devices and rendering the scene. Note that there can only be one RenderSystem active at the
/// time.
///
/// @ingroup render
class SEQUOIA_RENDER_API RenderSystem {
public:
  enum RenderSystemKind { RK_OpenGL };

  /// @brief Create the RenderSystem of the given `kind`
  /// @remark Terminates the program on failure
  static std::unique_ptr<RenderSystem> create(RenderSystemKind kind);

  /// @brief Terminate the render-system
  virtual ~RenderSystem() {}

  /// @brief Create a new RenderWindow
  ///
  /// @param title    The initial, UTF-8 encoded window title
  /// @returns the created window
  virtual RenderWindow* createWindow(const std::string& title) = 0;

  /// @brief Processes events that are in the event queue
  virtual void pollEvents() = 0;

  /// @brief Render one frame into the target of each active registered RenderTarget
  virtual void renderOneFrame() = 0;

  /// @brief Swap the buffers and display the next frame for each active registered RenderTarget
  virtual void swapBuffers() = 0;

  /// @brief Load a shader from source for `target`
  virtual Shader* loadShader(RenderTarget* target, const platform::Path& path) = 0;

  /// @brief Create a GPU program from the given shaders for `target`
  virtual GPUProgram* createProgram(RenderTarget* target, Shader* vertex, Shader* fragment) = 0;
};

} // namespace render

} // namespace sequoia

#endif
