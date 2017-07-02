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

#include "sequoia/Core/Export.h"
#include "sequoia/Core/File.h"
#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Math/Math.h"
#include "sequoia/Render/FrameListener.h"
#include "sequoia/Render/RenderFwd.h"
#include "sequoia/Render/Viewport.h"
#include <memory>
#include <unordered_map>

namespace sequoia {

namespace render {

class GLRenderWindow;
class GLShaderManager;
class GLProgramManager;
class GLTextureManager;
class GLStateCacheManager;

/// @brief OpenGL based renderer
///
/// A rendered performs the actual rendering to the associated target. Further, the renderer keeps
/// tabs on the OpenGL state machine and is it the only one who is allowed to perform OpenGL API
/// calls
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

  /// Cache of exentions
  std::unordered_map<std::string, bool> extensionCache_;

public:
  /// @brief Initialize the OpenGL context and bind it to the calling thread
  ///
  /// This also registers the renderer as a viewport listener of `window`.
  GLRenderer(GLRenderWindow* window);

  /// @brief Release the OpenGL context
  ~GLRenderer();

  /// @brief Render `target`
  void render(RenderTarget* target);

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

  /// @brief Adjust viewport
  virtual void viewportGeometryChanged(Viewport* viewport) override;

  /// @brief Check if `extension` is supported
  bool isExtensionSupported(const std::string& extension) noexcept;
};

} // namespace render

} // namespace sequoia

#endif
