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

#include "sequoia/Core/File.h"
#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Math/Math.h"
#include "sequoia/Render/Export.h"
#include <memory>

namespace sequoia {

namespace render {

class Shader;
class Program;
class GLRenderWindow;
class GLShaderManager;
class GLProgramManager;
class GLStateCacheManager;

/// @brief OpenGL based renderer
///
/// A rendered performs the actual rendering to the associated target. Further, the renderer keeps
/// tabs on the OpenGL state machine and is it the only one who is allowed to perform OpenGL API
/// calls
/// @ingroup gl
class SEQUOIA_RENDER_API GLRenderer : public NonCopyable {
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

public:
  /// @brief Initialize the OpenGL context and bind it to the calling thread
  GLRenderer(GLRenderWindow* target);

  /// @brief Release the OpenGL context
  ~GLRenderer();

  /// @brief Render the target
  void render();

  /// @brief Get the shader manager
  GLShaderManager* getShaderManager();

  /// @brief Get the program manager
  GLProgramManager* getProgramManager();

  /// @brief Get the OpenGL state manager
  GLStateCacheManager* getStateCache();

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
};

} // namespace render

} // namespace sequoia

#endif
