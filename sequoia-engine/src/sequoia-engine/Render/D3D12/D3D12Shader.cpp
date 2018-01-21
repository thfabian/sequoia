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
#include "sequoia-engine/Render/D3D12/D3D12Shader.h"

namespace sequoia {

namespace render {

D3D12Shader::D3D12Shader(ShaderType type, const std::string& filename, const std::string& source)
    : Shader(RK_D3D12, type), source_(source), filename_(filename) {}

D3D12Shader::~D3D12Shader() {}

const std::string& D3D12Shader::getFilename() const { return filename_; }

const std::string& D3D12Shader::getSourceCode() const { return source_; }

std::string D3D12Shader::toString() const {
  return core::format("D3D12Shader[\n"
                      "  filename = \"{}\"\n"
                      "]",
                      filename_);
}

void D3D12Shader::makeValidImpl() {}

} // namespace render

} // namespace sequoia
