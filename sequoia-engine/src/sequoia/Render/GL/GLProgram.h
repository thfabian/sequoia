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

#ifndef SEQUOIA_RENDER_GL_GLPROGRAM_H
#define SEQUOIA_RENDER_GL_GLPROGRAM_H

#include "sequoia/Render/GL/GLFwd.h"
#include "sequoia/Render/Program.h"
#include "sequoia/Render/UniformVariable.h"

#include <unordered_map>
#include <unordered_set>

namespace sequoia {

namespace render {

/// @brief OpenGL implementation of a shader program
/// @see RenderSystem::createPogram
/// @ingroup gl
class SEQUOIA_API GLProgram final : public Program {
public:
  /// @brief Represent uniform variables in shaders
  struct GLUniformInfo {
    GLenum Type;     ///< Type of the unifrom variable
    GLint Size;      ///< Size of the uniform variable
    GLint Location;  ///< Location of the uniform variable
    bool ValueSet;   ///< Check if the value of the uniform variable has been set
    int TextureUnit; ///< If this uniform corresponds to a texture sampler the associated texture
                     ///  unit is stored (-1 otherwise)
  };

  /// @brief Create an empty program object
  ///
  /// The shaders do not need to be valid.
  /// Programs should only be created via the factory method GLProgramManager::create.
  GLProgram(const std::set<std::shared_ptr<Shader>>& shaders);

  /// @brief Destroy the program
  ~GLProgram();

  /// @brief Get the unique identifer of the program
  ///
  /// Note that IDs might be reused after a program has been destroyed.
  unsigned int getID() const;

  /// @brief Bind the program to the current render pipline
  /// @note Do not call this function directly, use `GLStateCacheManager::bindProgram` instead
  void bind();

  /// @brief Unbind *any* program
  static void unbind();

  /// @brief Get map of uniform variables
  const std::unordered_map<std::string, GLUniformInfo>& getUniformVariables() const;
  std::unordered_map<std::string, GLUniformInfo>& getUniformVariables();

  /// @brief Set the uniform variable `name` to `value`
  ///
  /// @throws RenderSystemException   Variable `name` has incompatible type
  /// @returns `true` if variable has been successfully set, `false` if variable does not exist
  bool setUniformVariable(const std::string& name, const UniformVariable& variable);

  /// @brief Check if all uniform variables have been set
  bool checkUniformVariables();

  /// @brief Check if `name` is a uniform variable corresponding to a texture sampler
  bool isTextureSampler(const std::string& name) const;

  /// @brief Get the name of the texture sampler associated with `textureUnit` (empty string if no
  /// sampler is found)
  const std::string& getTextureSampler(int textureUnit) const;

  /// @copydoc Program::getShaders
  virtual const std::set<std::shared_ptr<Shader>>& getShaders() const override;

  /// @copydoc Program::toString
  virtual std::string toString() const override;

  /// @brief Issue a warning about setting a non-existing variable
  void reportWarningForInvalidUniformVariable(const std::string& name);

  /// @brief Empty string
  static const std::string EmptyString;

  /// @brief Destroy the program
  friend SEQUOIA_API void destroyGLProgram(GLProgram* program) noexcept;

  SEQUOIA_GL_OBJECT(Program)

protected:
  void makeValidImpl() override;

private:
  friend class GLProgramManager;

  /// OpenGL program index
  unsigned int id_;

  /// Map of uniform variables referenced in this program
  std::unordered_map<std::string, GLUniformInfo> uniformInfoMap_;

  /// Set of uniform variables which correspond to texture samplers
  std::unordered_map<int, std::string> textureSamplers_;

  /// Cache if all uniform variables have been set
  bool allUniformVariablesSet_;

  /// Cache if we already issued a warning for setting a non-existing variable
  std::unordered_set<std::string> reportedWarningForInvalidUniformVariable_;

  /// Shaders compiled into the program
  std::set<std::shared_ptr<Shader>> shaders_;
};

} // namespace render

} // namespace sequoia

#endif
