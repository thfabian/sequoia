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

#ifndef SEQUOIA_RENDER_GL_GLPROGRAM_H
#define SEQUOIA_RENDER_GL_GLPROGRAM_H

#include "sequoia/Math/Math.h"
#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/Program.h"
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

/// @brief Types of vertex Attributes
/// @ingroup gl
enum class GLVertexAttrib {
  VertexCoord = 0,
  Normal,
  TextureCoord,
  Tangent,
  Bitangent,
  VertexAttrib1,
  VertexAttrib2,
  VertexAttrib3,
  VertexAttrib4
};

/// @brief OpenGL implementation of a GPUProgram
/// @see RenderSystem::createPogram
/// @ingroup gl
class SEQUOIA_RENDER_API GLProgram : public Program {
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
  GLProgram(const std::set<Shader*>& shaders, GLProgramManager* manager);

  /// @brief Check if the program is valid i.e can be installed into the render pipeline
  bool isValid() const;

  /// @brief Get the unique identifer of the program
  ///
  /// Note that IDs might be reused after a program has been destroyed.
  unsigned int getID() const;

  /// @brief Get the status of the program
  GLProgramStatus getStatus() const;

  /// @brief Use the program in the current render pipline
  void use();

  /// @brief Add the `shader` to the program
  ///
  /// @note This forces a relink on next usage
  void addShader(Shader* shader);

  /// @brief Remove `shader` to the program
  ///
  /// @note This forces a relink on next usage
  /// @returns `true` if `shader` was successfully removed
  bool removeShader(Shader* shader);

  /// @brief Defines semantics for the input vertex attributes
  ///
  /// @param attrib  The semantic of the attribute
  /// @param name    The name of the vertex attribute
  void setVertexAttribName(GLVertexAttrib attrib, const std::string& name);

  /// @brief Get map of uniform variables
  const std::unordered_map<std::string, GLUniformInfo>& getUniformVariables() const;

  /// @brief Set the uniform variable `name` to `value`
  ///
  /// @throws RenderSystemException   Variable `name` has incompatible type
  /// @returns `true` if variable has been successfully set, `false` if variable does not exist
  /// @{
  bool setUniformVariable(const std::string& name, const float& value);
  bool setUniformVariable(const std::string& name, const int& value);
  bool setUniformVariable(const std::string& name, const math::fvec2& value);
  bool setUniformVariable(const std::string& name, const math::fvec3& value);
  bool setUniformVariable(const std::string& name, const math::fvec4& value);
  bool setUniformVariable(const std::string& name, const math::ivec2& value);
  bool setUniformVariable(const std::string& name, const math::ivec3& value);
  bool setUniformVariable(const std::string& name, const math::ivec4& value);
  bool setUniformVariable(const std::string& name, const math::fmat2& value);
  bool setUniformVariable(const std::string& name, const math::fmat3& value);
  bool setUniformVariable(const std::string& name, const math::fmat4& value);
  /// @}

  /// @brief Check if all uniform variables have been set
  bool checkUniformVariables();

  /// @brief Get the manager
  GLProgramManager* getManager() const;

  /// @copydoc Program::getShaders
  virtual const std::set<Shader*>& getShaders() const override;

  /// @copydoc Program::getLog
  virtual std::string getLog() const override;

  SEQUOIA_GL_OBJECT(Program);

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
  std::set<Shader*> shaders_;

  /// Reference to the manager
  GLProgramManager* manager_;
};

} // namespace render

} // namespace sequoia

#endif
