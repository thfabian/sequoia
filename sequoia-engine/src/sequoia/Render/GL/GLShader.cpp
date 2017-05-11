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

#include "sequoia/Render/GL/GLShader.h"

namespace sequoia {

namespace render {

GLShader::GLShader(Shader::ShaderType type, const platform::String& path)
    : Shader(type), status_(GLShaderStatus::OnDisk), id_(0), code_(), path_(path) {}

bool GLShader::isValid() const { return status_ == GLShaderStatus::Compiled; }

unsigned int GLShader::getID() const { return id_; }

platform::String GLShader::getSourcePath() const { return path_; }

std::string GLShader::getSourceCode() const { return code_; }

} // namespace render

} // namespace sequoia
