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

#include "sequoia/Core/Exception.h"
#include "sequoia/Core/Image.h"

#ifndef NDEBUG
#define STBI_FAILURE_USERMSG
#endif

#define STBI_SUPPORT_ZLIB
#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG
#define STBI_ONLY_BMP
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace sequoia {

namespace core {

Image::Image(const std::shared_ptr<File>& file) : file_(file) {
  pixelData_ = stbi_load_from_memory(file_->getData(), file_->getNumBytes(), &width_, &height_,
                                     &numChannels_, 0);
  if(!pixelData_)
    SEQUOIA_THROW(core::Exception, "failed to load image \"%s\": %s", file_->getPath(),
                  stbi_failure_reason());
}

Image::~Image() {
  if(pixelData_)
    stbi_image_free(pixelData_);
}

} // namespace core

} // namespace sequoia
