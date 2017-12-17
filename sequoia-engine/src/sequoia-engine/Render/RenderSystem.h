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

#ifndef SEQUOIA_ENGINE_RENDER_RENDERSYSTEM_H
#define SEQUOIA_ENGINE_RENDER_RENDERSYSTEM_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/Image.h"
#include "sequoia-engine/Core/Listenable.h"
#include "sequoia-engine/Core/NonCopyable.h"
#include "sequoia-engine/Core/Options.h"
#include "sequoia-engine/Core/Platform.h"
#include "sequoia-engine/Core/Singleton.h"
#include "sequoia-engine/Render/FrameListener.h"
#include "sequoia-engine/Render/Input.h"
#include "sequoia-engine/Render/RenderCommand.h"
#include "sequoia-engine/Render/RenderFwd.h"
#include "sequoia-engine/Render/RenderSystemObject.h"
#include "sequoia-engine/Render/RenderWindow.h"
#include "sequoia-engine/Render/Shader.h"
#include "sequoia-engine/Render/Texture.h"
#include "sequoia-engine/Render/VertexData.h"
#include <memory>
#include <set>
#include <string>

namespace sequoia {

namespace game {

class Game;

} // namespace game

namespace render {

/// @brief Currently active render-system
///
/// The render-system takes care of window and context creation as well as setting up the IO
/// devices and rendering the scene. Note that there can only be one RenderSystem active at the
/// time.
///
/// @ingroup render
class SEQUOIA_API RenderSystem : public Singleton<RenderSystem>,
                                 public FrameListener,
                                 public Listenable<InputEventListener, FrameListener>,
                                 public RenderSystemObject {
public:
  friend class sequoia::game::Game;

  /// @brief Create the Options with default values for all RenderSystem related options
  static std::shared_ptr<Options> makeOptions();

  /// @brief Create the RenderSystem of the given `kind`
  /// @remark Terminates the program on failure
  static std::unique_ptr<RenderSystem> create(RenderSystemKind kind,
                                              const std::shared_ptr<Options>& options);

  /// @brief Terminate the render-system
  virtual ~RenderSystem();

  /// @brief Create the main-window (if a main-window is already active, the old one will be
  /// destroyed first)
  ///
  /// For the OpenGL RenderSystem this also creates the OpenGL context.
  ///
  /// @returns the created window
  virtual RenderWindow* createMainWindow(const RenderWindow::WindowHint& hints) = 0;

  /// @brief Destroys the currently active main-window
  virtual void destroyMainWindow() noexcept = 0;

  /// @brief Get the main-window
  virtual RenderWindow* getMainWindow() const = 0;

  /// @brief Processes events that are in the event queue
  virtual void pollEvents() = 0;

  /// @brief Render one frame using the instructions given in `command`.
  void renderOneFrame(const RenderCommand& command);

  /// @brief Create a shader from source
  virtual std::shared_ptr<Shader> createShader(Shader::ShaderType type,
                                               const std::shared_ptr<File>& file) = 0;

  /// @brief Create a GPU program from the given `shaders`
  virtual std::shared_ptr<Program>
  createProgram(const std::set<std::shared_ptr<Shader>>& shaders) = 0;

  /// @brief Create a texture of `image` (using texture parameters `param`)
  virtual std::shared_ptr<Texture>
  createTexture(const std::shared_ptr<Image>& image,
                const TextureParameter& param = TextureParameter()) = 0;

  /// @brief Allocate vertex data
  virtual std::shared_ptr<VertexData> createVertexData(const VertexDataParameter& param) = 0;

  /// @brief Add the keyboard `listener`
  virtual void addKeyboardListener(KeyboardListener* listener) = 0;

  /// @brief Remove the keyboard `listener`
  virtual void removeKeyboardListener(KeyboardListener* listener) = 0;

  /// @brief Add the mouse `listener`
  virtual void addMouseListener(MouseListener* listener) = 0;

  /// @brief Remove the mouse `listener`
  virtual void removeMouseListener(MouseListener* listener) = 0;

  // TODO -- remove

  /// @brief Load the default vertex and fragment shaders and link them into a program
  ///
  /// @param defaultVertexShaderFile    File containing the default vertex shader
  /// @param defaultFragmentShaderFile  File containing the default fragment shader
  virtual void loadDefaultShaders(const std::shared_ptr<File>& defaultVertexShaderFile,
                                  const std::shared_ptr<File>& defaultFragmentShaderFile) = 0;

  /// @brief Get the default vertex shader
  virtual const std::shared_ptr<Shader>& getDefaultVertexShader() const = 0;

  /// @brief Get the default fragment shader
  virtual const std::shared_ptr<Shader>& getDefaultFragmentShader() const = 0;

  /// @brief Get the default fragment shader
  virtual const std::shared_ptr<Program>& getDefaultProgram() const = 0;

  // --------------------

  /// @brief Set if we run in debug-mode
  Options& getOptions() const { return *options_; }
  Options* getOptionsPtr() const { return options_.get(); }

  void frameListenerRenderingBegin(RenderCommand* command) override;
  void frameListenerRenderingEnd(RenderCommand* command) override;

protected:
  RenderSystem(RenderSystemKind kind, const std::shared_ptr<Options>& options);

  /// @brief Get the RenderStateManager
  ///
  /// This is used in `renderOneFrame()`.
  virtual RenderStateManager* getRenderStateManager() = 0;

private:
  std::shared_ptr<Options> options_;

private:
  static void setDefaultOptions(const std::shared_ptr<Options>& options);
};

} // namespace render

} // namespace sequoia

#endif
