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

#include "sequoia/Core/Casting.h"
#include "sequoia/Core/Exception.h"
#include "sequoia/Core/Format.h"
#include "sequoia/Core/HashCombine.h"
#include "sequoia/Core/Image.h"
#include "sequoia/Core/StringSwitch.h"
#include "sequoia/Core/Unreachable.h"
#include <gli/gli.hpp>
#include <memory>
#include <opencv2/opencv.hpp>

namespace sequoia {

namespace core {

Image::~Image() {}

const std::shared_ptr<File>& Image::getFile() const {
  SEQUOIA_ASSERT_MSG(hasFile(), "no file has been assigned to the image");
  return file_;
}

std::shared_ptr<Image> Image::load(const std::shared_ptr<File>& file) {
  switch(file->getType()) {
  case FileType::Png:
  case FileType::Jpeg:
  case FileType::Bmp:
    return std::make_shared<RegularImage>(file);
  case FileType::DDS:
    return std::make_shared<TextureImage>(file);
  default:
    SEQUOIA_THROW(Exception, "invalid image format of file: \"%s\"", file->getPath());
    return nullptr;
  }
}

Image::Image(Image::ImageKind kind, const std::shared_ptr<File>& file) : kind_(kind), file_(file) {}

//===------------------------------------------------------------------------------------------===//
//    RegularImage
//===------------------------------------------------------------------------------------------===//

RegularImage::RegularImage(int width, int height, ColorFormat format)
    : Image(IK_RegularImage, nullptr) {}

RegularImage::RegularImage(const std::shared_ptr<File>& file)
    : Image(IK_RegularImage, file) {

//  FREE_IMAGE_FORMAT FIFormat = GetFreeImageType(file->getType());
//  SEQUOIA_ASSERT_MSG(FIFormat != FIF_UNKNOWN, "invalid image format");
//  SEQUOIA_ASSERT_MSG(FreeImage_FIFSupportsReading(FIFormat),
//                     "format cannot be read (plugin not loaded)");

//  // Attach the binary data to a memory stream. Note that a memory buffer wrapped by FreeImage is
//  // read-only so const cast is safe here.
//  memory_ = FreeImage_OpenMemory(const_cast<Byte*>(file_->getData()), file_->getNumBytes());

//  // Load the image from the memory stream
//  bitMap_ = FreeImage_LoadFromMemory(FIFormat, memory_, 0);

//  // Check if the image is stored in RGB or BGR
//  bool colorOrderIsRGB = FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_RGB;

//  // Assert our image is 24 or 32 bits (8 bits per channel, Red/Green/Blue/Alpha)
//  int bitsPerPixel = FreeImage_GetBPP(bitMap_);
//  if(bitsPerPixel < 24) {
//    bitMapCopy_ = FreeImage_ConvertTo24Bits(bitMap_);
//    std::swap(bitMapCopy_, bitMap_);
//    colorFormat_ = colorOrderIsRGB ? ColorFormat::RGB : ColorFormat::BGR;

//  } else if(bitsPerPixel == 24) {
//    colorFormat_ = colorOrderIsRGB ? ColorFormat::RGB : ColorFormat::BGR;

//  } else if(bitsPerPixel == 32) {
//    colorFormat_ = colorOrderIsRGB ? ColorFormat::RGBA : ColorFormat::BGRA;
//  }

//  // Get bitmap informations
//  width_ = FreeImage_GetWidth(bitMap_);
//  height_ = FreeImage_GetHeight(bitMap_);

//  FREE_IMAGE_TYPE imageType = FreeImage_GetImageType(bitMap_);
//  FREE_IMAGE_COLOR_TYPE colorType = FreeImage_GetColorType(bitMap_);

//  SEQUOIA_ASSERT_MSG(colorType == FIC_RGB || colorType == FIC_RGBALPHA, "unsupported color type");
//  SEQUOIA_ASSERT_MSG(imageType == FIT_BITMAP, "unsupported image type");

//  // Get pixel data
//  pixelData_ = FreeImage_GetBits(bitMap_);
//  SEQUOIA_ASSERT(pixelData_);
}

RegularImage::~RegularImage() {}

const Byte* RegularImage::getPixelData() const { return image_->ptr(); }

Byte* RegularImage::getPixelData() { return image_->ptr(); }

Color RegularImage::at(int i, int j) const {
  return Color(colorFormat_, image_->ptr() + image_->channels() * (i * image_->rows + j));
}

int RegularImage::getWidth() const noexcept { return image_->rows; }

int RegularImage::getHeight() const noexcept { return image_->cols; }

std::string RegularImage::toString() const {
  return core::format("RegularImage[\n"
                      "  file = %s\n"
                      "  pixelData = %#016x\n"
                      "  width = %i,\n"
                      "  height = %i,\n"
                      "  numChannels = %i\n"
                      "]",
                      file_ ? file_->getPath() : "null", (std::size_t)getPixelData(), getWidth(),
                      getHeight(), getNumChannels());
}

bool RegularImage::equals(const Image* other) const noexcept {
  if(!Image::equals(other))
    return false;

  const RegularImage* thisImage = dyn_cast<RegularImage>(this);
  const RegularImage* otherImage = dyn_cast<RegularImage>(other);

  return thisImage->getPixelData() == thisImage->getPixelData() &&
         ((thisImage->hasFile() && otherImage->hasFile()) ? *thisImage->file_ == *otherImage->file_
                                                          : thisImage->file_ == otherImage->file_);
}

std::size_t RegularImage::hash() const noexcept {
  std::size_t seed = 0;
  if(hasFile())
    core::hashCombine(seed, file_->hash());
  core::hashCombine(seed, getPixelData());
  return seed;
}

//===------------------------------------------------------------------------------------------===//
//    TextureImage
//===------------------------------------------------------------------------------------------===//

TextureImage::TextureImage(const std::shared_ptr<File>& file) : Image(IK_TextureImage, file) {
  SEQUOIA_ASSERT_MSG(file_->getType() == FileType::DDS,
                     "only DDS can be loaded as texture image (yet)");

  // gli::load dispatches to the correct decoder
  image_ = std::make_unique<gli::texture>(
      gli::load(reinterpret_cast<char const*>(file_->getData()), file_->getNumBytes()));

  if(image_->empty())
    SEQUOIA_THROW(core::Exception, "failed to load texture image of file: %s", file_->getPath());
}

TextureImage::~TextureImage() {}

std::size_t TextureImage::hash() const noexcept {
  std::size_t seed = 0;
  core::hashCombine(seed, file_->hash(), std::hash<std::unique_ptr<gli::texture>>()(image_));
  return seed;
}

std::string TextureImage::toString() const {
  return core::format("TextureImage[\n"
                      "  file = %s,\n"
                      "  width = %i,\n"
                      "  height = %i,\n"
                      "  levels = %i\n"
                      "]",
                      file_->getPath(), image_->extent()[0], image_->extent()[1],
                      image_->levels());
}

bool TextureImage::equals(const Image* other) const noexcept {
  if(!Image::equals(other))
    return false;

  const TextureImage* thisImage = dyn_cast<TextureImage>(this);
  const TextureImage* otherImage = dyn_cast<TextureImage>(other);

  return *(thisImage->image_) == *(otherImage->image_) &&
         *(thisImage->file_) == *(otherImage->file_);
}

int TextureImage::getWidth() const noexcept { return image_->extent()[0]; }
int TextureImage::getHeight() const noexcept { return image_->extent()[1]; }

} // namespace core

} // namespace sequoia
