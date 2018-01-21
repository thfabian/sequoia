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

#include "sequoia-engine/Render/D3D12/D3D12Texture.h"
#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Core/StringUtil.h"

namespace sequoia {

namespace render {

D3D12Texture::D3D12Texture(const std::shared_ptr<Image>& image,
                         const std::shared_ptr<TextureParameter>& param)
    : Texture(RK_D3D12), image_(image), param_(param) {}

D3D12Texture::~D3D12Texture() {}

const std::shared_ptr<Image>& D3D12Texture::getImage() const { return image_; }

bool D3D12Texture::hasImage() const { return (image_ != nullptr); }

const std::shared_ptr<TextureParameter>& D3D12Texture::getParameter() const { return param_; }

void D3D12Texture::makeValidImpl() {}

std::string D3D12Texture::toString() const {
  return core::format("D3D12Texture[\n"
                      "  image = {},\n"
                      "  param = {}\n"
                      "]",
                      image_ ? core::indent(image_->toString()) : "d3d12",
                      core::indent(param_->toString()));
}

} // namespace render

} // namespace sequoia
