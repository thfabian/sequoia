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

#include "sequoia-engine/Render/GLSLShaderSource.h"

namespace sequoia {

namespace render {

GLSLShaderShource::GLSLShaderShource(ShaderSource::ShaderSourceKind kind) : ShaderSource(kind) {}

void GLSLShaderShource::loadFromString(const char* string, std::size_t size) {
  format_ = FK_String;
  data_.resize(size);
  std::memcpy(static_cast<void*>(data_.data()), static_cast<const void*>(string), size);
}

void GLSLShaderShource::loadFromFile(std::shared_ptr<File> file, FormatKind format) {}

} // namespace render

} // namespace sequoia
