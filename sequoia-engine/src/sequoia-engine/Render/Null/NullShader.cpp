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

#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Render/Null/NullShader.h"

namespace sequoia {

namespace render {

NullShader::NullShader(ShaderType type, const std::string& filename, const std::string& source)
    : Shader(RK_Null, type), source_(source), filename_(filename) {}

NullShader::~NullShader() {}

const std::string& NullShader::getFilename() const { return filename_; }

const std::string& NullShader::getSourceCode() const { return source_; }

std::string NullShader::toString() const {
  return core::format("NullShader[\n"
                      "  filename = \"{}\"\n"
                      "]",
                      filename_);
}

void NullShader::makeValidImpl() {}

} // namespace render

} // namespace sequoia
