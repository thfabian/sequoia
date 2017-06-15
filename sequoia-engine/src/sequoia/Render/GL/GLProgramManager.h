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
class SEQUOIA_API GLProgramManager : public NonCopyable {
  /// Record of all the registered programs (use count of 1 implies the program is *not* in use)
  std::vector<std::shared_ptr<GLProgram>> programList_;

  /// Lookup map for Programs
  std::unordered_map<std::size_t, std::size_t> shaderSetLookupMap_;

public:
  /// @brief Destroy all remaining programs
  ~GLProgramManager();

  /// @brief Create the shader from source and compile it
  ///
  /// @param type             Type of the shader
  /// @param path             Path to the shader source
  /// @param requestedStatus  Requested target status
  /// @throws RenderSystemException
  std::shared_ptr<GLProgram> create(const std::set<std::shared_ptr<Shader>>& shaders,
                                    GLProgramStatus requestedStatus = GLProgramStatus::Linked);

  /// @brief Convert the `program` to `status`
  /// @throws RenderSystemException
  void make(const std::shared_ptr<GLProgram>& program, GLProgramStatus requestedStatus);

  /// @brief Convert the shader to `GLProgramStatus::Linked`
  /// @see GLShaderLoader::make
  void makeValid(const std::shared_ptr<GLProgram>& program) {
    make(program, GLProgramStatus::Linked);
  }

  /// @brief Compute hash of the set of shaders
  static std::size_t hash(const std::set<std::shared_ptr<Shader>>& shaders) noexcept;

private:
  void getUniforms(const std::shared_ptr<GLProgram>& program) const;
  void setAttributes(const std::shared_ptr<GLProgram>& program) const;
  bool checkAttributes(const std::shared_ptr<GLProgram>& program) const;
};

} // namespace render

} // namespace sequoia

#endif
