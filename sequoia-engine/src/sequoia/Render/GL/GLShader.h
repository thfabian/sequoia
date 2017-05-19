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

#ifndef SEQUOIA_RENDER_GL_GLSHADER_H
#define SEQUOIA_RENDER_GL_GLSHADER_H

#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/Shader.h"

namespace sequoia {

namespace render {

class GLShaderManager;

/// @brief Status of an OpenGL shader
/// @ingroup gl
enum class GLShaderStatus {
  OnDisk = 0, ///< Shader has not been loaded from disk and path to shader may not exist
  InMemory,   ///< Shader was successfully loaded into memory
  Created,    ///< Shader has been registered within OpenGL and thus a unique ID has been assigned
  Compiled    ///< Shader has been successfully compiled and is ready for usage in programs
};

/// @brief OpenGL shader
/// @ingroup gl
class SEQUOIA_RENDER_API GLShader : public Shader {
  friend class GLShaderManager;

  /// Status of the shader
  GLShaderStatus status_;

  /// OpenGL shader index
  unsigned int id_;

  /// Reference to the manager
  GLShaderManager* manager_;

  /// Source code of the shader
  std::string code_;

  /// Source path the shader was loaded from
  platform::String path_;

public:
  /// @brief Get OpenGL shader type
  static GLenum getGLShaderType(ShaderType type);

  /// @brief Create the shader object by setting to path to its supposed location on disk
  ///
  /// Shaders should only be created via the factory method GLShaderManager::create.
  GLShader(ShaderType type, const platform::String& path, GLShaderManager* manager);

  /// @brief Check if the shader is valid i.e can be linked into a GPUProgram
  bool isValid() const;

  /// @brief Get the identifer of the shader
  ///
  /// Note that IDs might be reused after a shader has been destroyed.
  unsigned int getID() const;

  /// @copydoc Shader::getSourcePath
  platform::String getSourcePath() const override;

  /// @copydoc Shader::getSourceCode
  std::string getSourceCode() const override;

  /// @copydoc Shader::toString
  std::string toString() const override;

  /// @brief Get the status of the Shader
  GLShaderStatus getStatus() const;

  /// @brief Get the manager
  GLShaderManager* getManager() const;

  SEQUOIA_GL_OBJECT(Shader);
};

} // namespace render

} // namespace sequoia

#endif
