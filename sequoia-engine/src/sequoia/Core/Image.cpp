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

#include "sequoia/Core/Exception.h"
#include "sequoia/Core/Format.h"
#include "sequoia/Core/Image.h"
#include "sequoia/Core/StringSwitch.h"
#include "sequoia/Core/Unreachable.h"
#include "sequoia/Core/HashCombine.h"

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

Image::~Image() {}

std::shared_ptr<Image> Image::load(const std::shared_ptr<File>& file, Image::ImageFormat format) {
  if(format == Image::IK_Unknown)
    format = core::StringSwitch<Image::ImageFormat>(file->getExtension())
                 .Case(".png", Image::IK_PNG)
                 .Cases(".jpg", ".jpeg", ".jpe", Image::IK_JPEG)
                 .Cases(".bmp", ".dib", Image::IK_BMP)
                 .Default(Image::IK_Unknown);

  switch(format) {
  case Image::IK_PNG:
    return std::make_shared<PNGImage>(file);
  case Image::IK_JPEG:
    return std::make_shared<JPEGImage>(file);
  case Image::IK_BMP:
    return std::make_shared<BMPImage>(file);
  default:
    sequoia_unreachable("invalid image format");
  }
}

Image::Image(Image::ImageFormat format) : format_(format) {}

UncompressedImage::UncompressedImage(ImageFormat format, const std::shared_ptr<File>& file)
    : Image(format), file_(file) {
  pixelData_ = stbi_load_from_memory(file_->getData(), file_->getNumBytes(), &width_, &height_,
                                     &numChannels_, 0);
  if(!pixelData_)
    SEQUOIA_THROW(core::Exception, "failed to load image \"%s\": %s", file_->getPath(),
                  stbi_failure_reason());
}

UncompressedImage::~UncompressedImage() {
  if(pixelData_)
    stbi_image_free(pixelData_);
}

std::string UncompressedImage::toString() const {
  return core::format("%s[\n"
                      "  file = %s\n"
                      "  pixelData = %s\n"
                      "  width = %i,\n"
                      "  height = %i,\n"
                      "  numChannels = %i\n"
                      "]",
                      getName(), file_->getPath(), pixelData_, width_, height_, numChannels_);
}

std::size_t UncompressedImage::hash() const noexcept {
  std::size_t seed = 0;
  core::hashCombine(seed, file_->hash(), pixelData_, width_, height_, numChannels_);
  return seed;
}

PNGImage::PNGImage(const std::shared_ptr<File>& file) : UncompressedImage(Image::IK_PNG, file) {}

JPEGImage::JPEGImage(const std::shared_ptr<File>& file) : UncompressedImage(Image::IK_JPEG, file) {}

BMPImage::BMPImage(const std::shared_ptr<File>& file) : UncompressedImage(Image::IK_BMP, file) {}

} // namespace core

} // namespace sequoia
