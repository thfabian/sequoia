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

#include "sequoia/Render/GL/GLTexture.h"

namespace sequoia {

namespace render {

GLTexture::~GLTexture() {}

const std::shared_ptr<File>& GLTexture::getFile() const { return image_->getFile(); }

std::string GLTexture::getLog() const { return std::string(); }

std::string GLTexture::toString() const { return std::string(); }

} // render

} // namespace sequoia
