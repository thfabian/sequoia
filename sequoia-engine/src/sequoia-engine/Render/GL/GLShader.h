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

#ifndef SEQUOIA_ENGINE_RENDER_GL_GLSHADER_H
#define SEQUOIA_ENGINE_RENDER_GL_GLSHADER_H

#include "sequoia-engine/Render/GL/GLFwd.h"
#include "sequoia-engine/Render/Shader.h"

namespace sequoia {

namespace render {

/// @brief OpenGL shader implementation
/// @ingroup gl
class SEQUOIA_API GLShader final : public Shader {
  friend class GLShaderManager;

  /// OpenGL shader index
  unsigned int id_;

  /// Copy of the (optimized) source code of the shader
  std::string code_;

  /// File the shader was loaded from
  std::shared_ptr<File> file_;

public:
  /// @brief Create an empty shader object
  GLShader(ShaderType type, const std::shared_ptr<File>& file);

  /// @brief Destroy the shader
  virtual ~GLShader();

  /// @brief Get the identifer of the shader
  ///
  /// Note that IDs might be reused after a shader has been destroyed.
  unsigned int getID() const;

  /// @copydoc Shader::getFile
  virtual const std::shared_ptr<File>& getFile() const override;

  /// @copydoc Shader::getSourceCode
  virtual std::string getSourceCode() const override;

  /// @copydoc Shader::toString
  virtual std::string toString() const override;

  /// @brief Destroy the shader by deregistering it from OpenGL (i.e it's status will be changed to
  /// `InMemory`)
  friend SEQUOIA_API void destroyGLShader(GLShader* shader) noexcept;

  /// @brief Get OpenGL shader type
  static GLenum getGLShaderType(ShaderType type);

  SEQUOIA_GL_OBJECT(Shader)

protected:
  void makeValidImpl() override;
};

} // namespace render

} // namespace sequoia

#endif
