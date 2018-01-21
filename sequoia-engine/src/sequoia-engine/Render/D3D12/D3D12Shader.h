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

#ifndef SEQUOIA_ENGINE_RENDER_D3D12_D3D12SHADER_H
#define SEQUOIA_ENGINE_RENDER_D3D12_D3D12SHADER_H

#include "sequoia-engine/Render/Shader.h"

namespace sequoia {

namespace render {

/// @brief D3D12 shader implementation
/// @ingroup d3d12
class SEQUOIA_API D3D12Shader final : public Shader {

  /// Copy of the source code of the shader
  std::string source_;

  /// File the shader was loaded from
  std::string filename_;

public:
  /// @brief Create an empty shader object
  D3D12Shader(ShaderType type, const std::string& filename, const std::string& source);

  /// @brief Destroy the shader
  virtual ~D3D12Shader();

  /// @copydoc Shader::getFilename
  virtual const std::string& getFilename() const override;

  /// @copydoc Shader::getSourceCode
  virtual const std::string& getSourceCode() const override;

  /// @copydoc Shader::toString
  virtual std::string toString() const override;

  SEQUOIA_D3D12_OBJECT(Shader)

protected:
  virtual void makeValidImpl() override;
};

} // namespace render

} // namespace sequoia

#endif
