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

#ifndef SEQUOIA_RENDER_GL_GLRENDERER_H
#define SEQUOIA_RENDER_GL_GLRENDERER_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/File.h"
#include "sequoia-engine/Core/NonCopyable.h"
#include "sequoia-engine/Render/FrameListener.h"
#include "sequoia-engine/Render/GL/GLFwd.h"
#include "sequoia-engine/Render/RenderFwd.h"
#include "sequoia-engine/Render/Viewport.h"
#include <memory>
#include <unordered_map>

namespace sequoia {

namespace render {

/// @brief OpenGL based renderer
///
/// A rendered performs the actual rendering of the provided target. Further, the renderer keeps
/// tabs on the OpenGL state machine.
/// @ingroup gl
class SEQUOIA_API GLRenderer : public ViewportListener,
                               public Listenable<FrameListener>,
                               public NonCopyable {
  /// Reference to the main-window
  GLRenderWindow* window_;

  /// Default shaders
  std::shared_ptr<Shader> defaultVertexShader_;
  std::shared_ptr<Shader> defaultFragmentShader_;

  /// Default GPU Program
  std::shared_ptr<Program> defaultProgram_;

  /// Managers
  std::unique_ptr<GLStateCacheManager> stateCacheManager_;
  std::unique_ptr<GLShaderManager> shaderManager_;
  std::unique_ptr<GLProgramManager> programManager_;
  std::unique_ptr<GLTextureManager> textureManager_;
  std::unique_ptr<GLExtensionManager> extensionManager_;

  /// Cache of exentions
  std::unordered_map<std::string, bool> extensionCache_;

public:
  /// @brief Initialize the OpenGL context and bind it to the calling thread
  ///
  /// This also registers the renderer as a viewport listener of `window`.
  GLRenderer(GLRenderWindow* window);

  /// @brief Release the OpenGL context
  ~GLRenderer();

  /// @brief Render `command`
  void render(RenderCommand* command);

  /// @brief Get the shader manager
  GLShaderManager* getShaderManager();

  /// @brief Get the program manager
  GLProgramManager* getProgramManager();

  /// @brief Get the texture manager
  GLTextureManager* getTextureManager();

  /// @brief Get the OpenGL state manager
  GLStateCacheManager* getStateCacheManager();

  /// @brief Load the default vertex and fragment shaders and link them into a program of `target`
  ///
  /// @param defaultVertexShaderFile    File containing the default vertex shader
  /// @param defaultFragmentShaderFile  File containing the default fragment shader
  void loadDefaultShaders(const std::shared_ptr<File>& defaultVertexShaderFile,
                          const std::shared_ptr<File>& defaultFragmentShaderFile);

  /// @brief Get the default vertex shader
  const std::shared_ptr<Shader>& getDefaultVertexShader() const;

  /// @brief Get the default fragment shader
  const std::shared_ptr<Shader>& getDefaultFragmentShader() const;

  /// @brief Get the default fragment shader
  const std::shared_ptr<Program>& getDefaultProgram() const;

  /// @brief Check if `extension` is supported
  /// @see GLExtensionManager::supported
  bool isExtensionSupported(gl::GLextension extension) noexcept;

  /// @brief Adjust viewport
  virtual void viewportGeometryChanged(Viewport* viewport) override;
};

/// @brief Get a reference to the currently active `GLRenderer`
/// @ingroup gl
/// @{
SEQUOIA_API extern GLRenderer& getGLRenderer() noexcept;
SEQUOIA_API extern GLRenderer* getGLRendererPtr() noexcept;
/// @}

} // namespace render

} // namespace sequoia

#endif
