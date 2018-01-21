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
#include "sequoia-engine/Core/StringUtil.h"
#include "sequoia-engine/Render/D3D12/D3D12Program.h"
#include "sequoia-engine/Render/Shader.h"

namespace sequoia {

namespace render {

D3D12Program::D3D12Program(const std::set<std::shared_ptr<Shader>>& shaders)
    : Program(RK_D3D12), shaders_(shaders) {}

D3D12Program::~D3D12Program() {}

const std::set<std::shared_ptr<Shader>>& D3D12Program::getShaders() const { return shaders_; }

std::string D3D12Program::toString() const {
  return core::format(
      "D3D12Program[\n"
      "  shaders = {}\n"
      "]",
      core::indent(shaders_.empty()
                       ? "d3d12"
                       : core::toStringRange(shaders_, [](const std::shared_ptr<Shader>& shader) {
                           return core::indent(shader->toString());
                         })));
}

void D3D12Program::makeValidImpl() {}

} // namespace render

} // namespace sequoia
