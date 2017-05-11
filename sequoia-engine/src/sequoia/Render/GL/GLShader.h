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

#include "sequoia/Core/EnumState.h"
#include "sequoia/Core/Unreachable.h"
#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/Shader.h"

namespace sequoia {

namespace render {

class GLShaderManager;

/// @brief Status of the OpenGL shader
enum class GLShaderStatus {
  OnDisk = 0, ///< Shader has not been loaded from disk and path to shader may not exist
  InMemory,   ///< Shader was successfully loaded into memory
  Created,    ///< Shader has been registered within OpenGL and thus a unique ID has been assigned
  Compiled    ///< Shader has been successfully compiled and is ready for usage in programs
};

using GLShaderStatusState =
    core::EnumState<GLShaderStatus, GLShaderStatus::OnDisk, GLShaderStatus::InMemory,
                    GLShaderStatus::Created, GLShaderStatus::Compiled>;

inline GLShaderStatus operator++(GLShaderStatus& s, int) {
  auto sold = s;
  GLShaderStatusState::advance(s);
  return sold;
}

/// @brief OpenGL shader
/// @ingroup gl
class SEQUOIA_RENDER_API GLShader : public Shader {
  friend class GLShaderManager;

  /// Status of the shader
  GLShaderStatus status_;

  /// OpenGL shader index
  unsigned int id_;

  /// Source code of the shader
  std::string code_;

  /// Source path the shader was loaded from
  platform::String path_;

public:
  /// @brief Get OpenGL shader type
  static GLenum getGLShaderType(ShaderType s) {
    switch(s) {
    case ST_Compute:
      return GL_COMPUTE_SHADER;
    case ST_Vertex:
      return GL_VERTEX_SHADER;
    case ST_TessControl:
      return GL_TESS_CONTROL_SHADER;
    case ST_TessEvaluation:
      return GL_TESS_EVALUATION_SHADER;
    case ST_Geometry:
      return GL_GEOMETRY_SHADER;
    case ST_Fragment:
      return GL_FRAGMENT_SHADER;
    default:
      sequoia_unreachable("invalid ShaderType");
    }
  }

  /// @brief Create the shader object by setting to path to its supposed location on disk
  ///
  /// Shaders should only be created via the factory method GLShaderManager::create.
  GLShader(ShaderType type, const platform::String& path);

  /// @brief Check if the shader is valid i.e can be linked into a GPUProgram
  bool isValid() const;

  /// @copydoc Shader::getID
  unsigned int getID() const override;

  /// @copydoc Shader::getSourcePath
  platform::String getSourcePath() const override;

  /// @copydoc Shader::getSourceCode
  std::string getSourceCode() const override;
};

} // namespace render

} // namespace sequoia

#endif
