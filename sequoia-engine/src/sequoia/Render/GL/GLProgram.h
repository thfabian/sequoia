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

#include "sequoia/Math/Math.h"
#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/Program.h"
#include "sequoia/Render/UniformVariable.h"
#include <unordered_map>

namespace sequoia {

namespace render {

class GLProgramManager;

/// @brief Status of an OpenGL program
/// @ingroup gl
enum class GLProgramStatus {
  Invalid = 0,
  Created, ///< Program was registered within OpenGL and recieved a unique ID
  Linked,  ///< Program was successfully linked and is usable
};

/// @brief OpenGL implementation of a GPUProgram
/// @see RenderSystem::createPogram
/// @ingroup gl
class SEQUOIA_API GLProgram final : public Program {
public:
  /// @brief Represent uniform variables in shaders
  struct GLUniformInfo {
    GLenum Type;    ///< Type of the unifrom variable
    GLint Size;     ///< Size of the uniform variable
    GLint Location; ///< Location of the uniform variable
    bool ValueSet;  ///< Check if the value of the uniform variable has been set
  };

  /// @brief Create the program (this tries to make every shader valid)
  ///
  /// Programs should only be created via the factory method GLProgramManager::create.
  GLProgram(const std::set<std::shared_ptr<Shader>>& shaders, GLProgramManager* manager);

  /// @brief Destroy the program
  ~GLProgram();

  /// @brief Check if the program is valid i.e can be installed into the render pipeline
  bool isValid() const;

  /// @brief Get the unique identifer of the program
  ///
  /// Note that IDs might be reused after a program has been destroyed.
  unsigned int getID() const;

  /// @brief Get the status of the program
  GLProgramStatus getStatus() const;

  /// @brief Bind the program to the current render pipline
  void bind();

  /// @brief Unbind the program
  void unbind();

  /// @brief Add the `shader` to the program
  ///
  /// @note This forces a relink on next usage
  void addShader(const std::shared_ptr<Shader>& shader);

  /// @brief Remove `shader` to the program
  ///
  /// @note This forces a relink on next usage
  /// @returns `true` if `shader` was successfully removed
  bool removeShader(const std::shared_ptr<Shader>& shader);

  /// @brief Get map of uniform variables
  const std::unordered_map<std::string, GLUniformInfo>& getUniformVariables() const;
  std::unordered_map<std::string, GLUniformInfo> getUniformVariables();

  /// @brief Set the uniform variable `name` to `value`
  ///
  /// @throws RenderSystemException   Variable `name` has incompatible type
  /// @returns `true` if variable has been successfully set, `false` if variable does not exist
  bool setUniformVariable(const std::string& name, const UniformVariable& value);

  /// @brief Check if all uniform variables have been set
  bool checkUniformVariables();

  /// @brief Get the manager
  GLProgramManager* getManager() const;

  /// @copydoc Program::getShaders
  virtual const std::set<std::shared_ptr<Shader>>& getShaders() const override;

  /// @copydoc Program::getLog
  virtual std::string getLog() const override;

  /// @copydoc Program::toString
  virtual std::string toString() const override;

  /// @brief Destroy the program
  friend SEQUOIA_API void destroyGLProgram(GLProgram* program) noexcept;

  SEQUOIA_GL_OBJECT(Program)

private:
  friend class GLProgramManager;

  /// Status of the program
  GLProgramStatus status_;

  /// OpenGL program index
  unsigned int id_;

  /// Map of uniform variables referenced in this program
  std::unordered_map<std::string, GLUniformInfo> uniformInfoMap_;

  /// Cache if all uniform variables have been set
  bool allUniformVariablesSet_;

  /// Shaders compiled into the program
  std::set<std::shared_ptr<Shader>> shaders_;

  /// Reference to the manager
  GLProgramManager* manager_;
};

} // namespace render

} // namespace sequoia

#endif
