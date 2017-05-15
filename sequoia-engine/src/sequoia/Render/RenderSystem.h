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

#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Core/Platform.h"
#include "sequoia/Core/Singleton.h"
#include "sequoia/Render/Export.h"
#include "sequoia/Render/RenderFwd.h"
#include "sequoia/Render/RenderSystemObject.h"
#include "sequoia/Render/RenderWindow.h"
#include "sequoia/Render/Shader.h"
#include <memory>
#include <set>
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
class SEQUOIA_RENDER_API RenderSystem : public Singleton<RenderSystem>, public RenderSystemObject {
public:
  /// @brief Create the RenderSystem of the given `kind`
  /// @remark Terminates the program on failure
  static std::unique_ptr<RenderSystem> create(RenderSystemKind kind);

  /// @brief Terminate the render-system
  virtual ~RenderSystem() {}

  /// @brief Create a new RenderWindow
  /// @returns the created window
  virtual RenderWindow* createWindow(const RenderWindow::WindowHint& hints) = 0;

  /// @brief Manually destroy the RenderTarget
  virtual void destroyTarget(RenderTarget* target) = 0;

  /// @brief Processes events that are in the event queue
  virtual void pollEvents() = 0;

  /// @brief Render one frame into the target of each active registered RenderTarget
  virtual void renderOneFrame() = 0;

  /// @brief Swap the buffers and display the next frame for each active registered RenderTarget
  virtual void swapBuffers() = 0;

  /// @brief Load a shader from source for `target`
  virtual Shader* loadShader(RenderTarget* target, Shader::ShaderType type,
                             const platform::String& path) = 0;

  /// @brief Destroy the `shader` of `target`
  virtual void destroyShader(RenderTarget* target, Shader* shader) = 0;

  /// @brief Create a GPU program from the given `shaders` for `target`
  virtual Program* createProgram(RenderTarget* target, const std::set<Shader*>& shaders) = 0;

  /// @brief Destroy the `program` of `target`
  virtual void destroyProgram(RenderTarget* target, Program* program) = 0;

  /// @brief Set if we run in debug-mode (needs to be set before creating windows/targets to take
  /// full effect)
  void setDebugMode(bool debugMode);

  /// @brief Check if we run in debug-mode
  bool debugMode() const;

protected:
  RenderSystem(RenderSystemKind kind);

private:
  bool debugMode_;
};

} // namespace render

} // namespace sequoia

#endif
