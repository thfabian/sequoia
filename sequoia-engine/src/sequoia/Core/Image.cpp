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

#include "sequoia/Core/Image.h"
#include "sequoia/Core/Casting.h"
#include "sequoia/Core/Exception.h"
#include "sequoia/Core/Format.h"
#include "sequoia/Core/HashCombine.h"
#include "sequoia/Core/StringSwitch.h"
#include "sequoia/Core/Unreachable.h"

#ifndef NDEBUG
#define STBI_FAILURE_USERMSG
#endif

#define STBI_SUPPORT_ZLIB
#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG
#define STBI_ONLY_BMP
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>

namespace sequoia {

namespace core {

Image::~Image() {}

std::shared_ptr<Image> Image::load(const std::shared_ptr<File>& file, Image::ImageFormat format) {
  if(format == Image::IF_Unknown)
    format = core::StringSwitch<Image::ImageFormat>(file->getExtension())
                 .Case(".png", Image::IF_PNG)
                 .Cases(".jpg", ".jpeg", ".jpe", Image::IF_JPEG)
                 .Cases(".bmp", ".dib", Image::IF_BMP)
                 .Default(Image::IF_Unknown);


  switch(format) {
  case Image::IF_PNG:
    return std::make_shared<PNGImage>(file);
  case Image::IF_JPEG:
    return std::make_shared<JPEGImage>(file);
  case Image::IF_BMP:
    return std::make_shared<BMPImage>(file);
  default:
    SEQUOIA_THROW(Exception, "invalid image format of file: \"%s\"", file->getPath());
    return nullptr;
  }
}

Image::Image(Image::ImageFormat format) : format_(format) {}

UncompressedImage::UncompressedImage(ImageFormat format, const std::shared_ptr<File>& file)
    : Image(format), file_(file) {

  // TODO: STB is not reentrant ... we need to use different library for image loading
  //       Probably best to use the native libraries (libpng, libjpeg, ...) or DevIL
  int numChannels = 0;
  pixelData_ = stbi_load_from_memory(file_->getData(), file_->getNumBytes(), &width_, &height_,
                                     &numChannels, 0);
  if(!pixelData_)
    SEQUOIA_THROW(core::Exception, "failed to load image \"%s\": %s", file_->getPath(),
                  stbi_failure_reason());

  switch(numChannels) {
  case 1:
    colorFormat_ = ColorFormat::R;
    break;
  case 2:
    colorFormat_ = ColorFormat::RG;
    break;
  case 3:
    colorFormat_ = ColorFormat::RGB;
    break;
  case 4:
    colorFormat_ = ColorFormat::RGBA;
    break;
  }
}

UncompressedImage::~UncompressedImage() {
  if(pixelData_)
    stbi_image_free(pixelData_);
}

std::string UncompressedImage::toString() const {
  return core::format("%s[\n"
                      "  file = %s\n"
                      "  pixelData = %#016x\n"
                      "  width = %i,\n"
                      "  height = %i,\n"
                      "  colorFormat = %s,\n"
                      "  numChannels = %i\n"
                      "]",
                      getName(), file_->getPath(), (std::size_t)pixelData_, width_, height_,
                      colorFormat_, getNumChannels());
}

bool UncompressedImage::equals(const Image* other) const noexcept {
  if(!Image::equals(other))
    return false;

  const UncompressedImage* thisImage = dyn_cast<UncompressedImage>(this);
  const UncompressedImage* otherImage = dyn_cast<UncompressedImage>(other);

  return thisImage->pixelData_ == otherImage->pixelData_ &&
         thisImage->width_ == otherImage->width_ && thisImage->height_ == otherImage->height_ &&
         thisImage->colorFormat_ == otherImage->colorFormat_ &&
         *(thisImage->file_) == *(otherImage->file_);
}

std::size_t UncompressedImage::hash() const noexcept {
  std::size_t seed = 0;
  core::hashCombine(seed, file_->hash(), pixelData_, width_, height_, colorFormat_);
  return seed;
}

PNGImage::PNGImage(const std::shared_ptr<File>& file) : UncompressedImage(Image::IF_PNG, file) {}

JPEGImage::JPEGImage(const std::shared_ptr<File>& file) : UncompressedImage(Image::IF_JPEG, file) {}

BMPImage::BMPImage(const std::shared_ptr<File>& file) : UncompressedImage(Image::IF_BMP, file) {}

} // namespace core

} // namespace sequoia
