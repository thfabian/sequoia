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

#ifndef SEQUOIA_ENGINE_RENDER_GLSLSHADERSOURCE_H
#define SEQUOIA_ENGINE_RENDER_GLSLSHADERSOURCE_H

#include "sequoia-engine/Core/AlignedADT.h"
#include "sequoia-engine/Core/Byte.h"
#include "sequoia-engine/Core/File.h"
#include "sequoia-engine/Render/ShaderSource.h"

namespace sequoia {

namespace render {

/// @brief GLSL shader source wrapper
/// @ingroup render
class SEQUOIA_API GLSLShaderShource final : public ShaderSource {
public:
  enum FormatKind {
    FK_SpirV,
    FK_String,
  };

  /// @brief Initialize an empty shader
  GLSLShaderShource(ShaderSource::ShaderSourceKind kind);

  /// @brief Load shader source from by copying from `string`
  /// @{
  void loadFromString(const char* string, std::size_t size);
  void loadFromString(const std::string& string) { loadFromString(string.data(), string.size()); }
  /// @}

  /// @brief Load shader source from `file`
  void loadFromFile(std::shared_ptr<File> file, FormatKind format);

  /// @brief Get the format of the source
  FormatKind getFormat() const noexcept { return format_; }

  /// @brief Get the shader source data
  const aligned_vector<Byte>& getData() const noexcept { return data_; }

  static bool classof(const ShaderSource* source) { return source->getKind() == SK_GLSL; }

private:
  /// Shader data
  aligned_vector<Byte> data_;

  /// Format of the data
  FormatKind format_;
};

} // namespace render

} // namespace sequoia

#endif