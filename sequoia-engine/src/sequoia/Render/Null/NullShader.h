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

#ifndef SEQUOIA_RENDER_NULL_NULLSHADER_H
#define SEQUOIA_RENDER_NULL_NULLSHADER_H

#include "sequoia/Render/Shader.h"

namespace sequoia {

namespace render {

/// @brief Null shader implementation
/// @ingroup null
class SEQUOIA_API NullShader final : public Shader {

  /// Copy of the source code of the shader
  std::string code_;

  /// File the shader was loaded from
  std::shared_ptr<File> file_;

public:
  /// @brief Create an empty shader object
  NullShader(ShaderType type, const std::shared_ptr<File>& file);

  /// @brief Destroy the shader
  virtual ~NullShader();

  /// @copydoc Shader::getFile
  virtual const std::shared_ptr<File>& getFile() const override;

  /// @copydoc Shader::getSourceCode
  virtual std::string getSourceCode() const override;

  /// @copydoc Shader::toString
  virtual std::string toString() const override;

  SEQUOIA_NULL_OBJECT(Shader)

protected:
  virtual void makeValidImpl() override;
};

} // namespace render

} // namespace sequoia

#endif
