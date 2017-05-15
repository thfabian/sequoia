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

#ifndef SEQUOIA_RENDER_GL_GLPROGRAMMANAGER_H
#define SEQUOIA_RENDER_GL_GLPROGRAMMANAGER_H

#include "sequoia/Core/Assert.h"
#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Render/Export.h"
#include "sequoia/Render/GL/GLProgram.h"
#include <memory>
#include <unordered_map>
#include <vector>

namespace sequoia {

namespace render {

class GLShaderManager;
class GLShader;

/// @brief Create and compile OpenGL GPU programs
/// @ingroup gl
class SEQUOIA_RENDER_API GLProgramManager : public NonCopyable {
public:
  /// @brief Destroy all remaining programs
  ~GLProgramManager();

  /// @brief Create the shader from source and compile it
  ///
  /// @param type             Type of the shader
  /// @param path             Path to the shader source
  /// @param requestedStatus  Requested target status
  /// @throws RenderSystemException
  GLProgram* create(const std::set<Shader*>& shaders,
                    GLProgramStatus requestedStatus = GLProgramStatus::Linked);

  /// @brief Convert the program to `status`
  /// @throws RenderSystemException
  void make(GLProgram* program, GLProgramStatus requestedStatus);

  /// @brief Destroy the program by deregistering it from OpenGL
  void destroy(GLProgram* program);

  /// @brief Convert the shader to `GLProgramStatus::Linked`
  /// @see GLShaderLoader::make
  void makeValid(GLProgram* program) { make(program, GLProgramStatus::Linked); }

  /// @brief Compute hash of set of shaders
  static std::size_t hash(const std::set<Shader*>& shaders) noexcept;

private:
  /// Record of all the registered programs
  std::vector<std::unique_ptr<GLProgram>> programList_;

  /// Lookup map for Programs
  std::unordered_map<std::size_t, std::size_t> shaderSetLookupMap_;
};

} // namespace render

} // namespace sequoia

#endif
