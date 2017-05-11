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

#include "sequoia/Render/Shader.h"

namespace sequoia {

namespace render {

/// @brief OpenGL shader representation
/// @ingroup gl
class SEQUOIA_RENDER_API GLShader : public Shader {
  unsigned int id_;
  ShaderType type_; 

  //
  // TODO: !!! ShaderRecord should be Shader itself !!!
  //

private:
  GLShader(unsigned int id);

  /// @copydoc Shader::getID
  virtual unsigned int getID() const override;

  /// @copydoc Shader::getType
  virtual ShaderType getType() const override;

  /// @copydoc Shader::getSourcePath
  platform::String getSourcePath() const override;

  /// @copydoc Shader::getSourceCode
  std::string getSourceCode() const override;
};

} // namespace render

} // namespace sequoia

#endif
