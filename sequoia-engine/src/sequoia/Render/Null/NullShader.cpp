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

#include "sequoia/Core/Format.h"
#include "sequoia/Core/UtfString.h"
#include "sequoia/Render/Null/NullShader.h"

namespace sequoia {

namespace render {

NullShader::NullShader(Shader::ShaderType type, const std::shared_ptr<File>& file)
    : Shader(RK_Null, type), file_(file) {
  code_ = file->getDataAsString();
}

NullShader::~NullShader() {}

const std::shared_ptr<File>& NullShader::getFile() const { return file_; }

std::string NullShader::getSourceCode() const { return code_; }

std::string NullShader::toString() const {
  return core::format("NullShader[\n"
                      "  path = \"%s\"\n"
                      "]",
                      UtfString(file_->getPath()).toAnsiString());
}

void NullShader::makeValidImpl() {}

} // namespace render

} // namespace sequoia
