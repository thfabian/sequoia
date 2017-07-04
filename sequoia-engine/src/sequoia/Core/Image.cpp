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
#include <FreeImage/FreeImage.h>
#include <gli/gli.hpp>
#include <memory>
#include <mutex>

namespace sequoia {

namespace core {

namespace {

/// @brief Context of the FreeImage library
struct FreeImageContext {
  FreeImageContext() {
    FreeImage_Initialise();
    FreeImage_SetOutputMessage(FreeImageContext::ErrorHandler);
  }

  ~FreeImageContext() { FreeImage_DeInitialise(); }

  /// @brief Translate FreeImage error into an exception
  static void ErrorHandler(FREE_IMAGE_FORMAT format, const char* message) {
    SEQUOIA_THROW(core::Exception, "failed to load image: format=%s : %s",
                  format != FIF_UNKNOWN ? FreeImage_GetFormatFromFIF(format) : "unknown", message);
  }
};

std::unique_ptr<FreeImageContext> freeImageContext = nullptr;
std::once_flag freeImageContextInitFlag;

static FREE_IMAGE_FORMAT GetFreeImageType(Image::ImageFormat format) {
  switch(format) {
  case Image::IF_PNG:
    return FIF_PNG;
  case Image::IF_JPEG:
    return FIF_JPEG;
  case Image::IF_BMP:
    return FIF_BMP;
  default:
    return FIF_UNKNOWN;
  }
}

} // anonymous namespace

Image::~Image() {}

const std::shared_ptr<File>& Image::getFile() const {
  SEQUOIA_ASSERT_MSG(hasFile(), "no file has been assigend to the image");
  return file_;
}

std::shared_ptr<Image> Image::load(const std::shared_ptr<File>& file) {
  std::call_once(freeImageContextInitFlag,
                 []() { freeImageContext = std::make_unique<FreeImageContext>(); });

  switch(file->getType()) {
  case FileType::Png:
    return std::make_shared<RegularImage>(IF_PNG, file);
  case FileType::Jpeg:
    return std::make_shared<RegularImage>(IF_JPEG, file);
  case FileType::Bmp:
    return std::make_shared<RegularImage>(IF_BMP, file);
  case FileType::DDS:
    return std::make_shared<TextureImage>(IF_DDS, file);
  default:
    SEQUOIA_THROW(Exception, "invalid image format of file: \"%s\"", file->getPath());
    return nullptr;
  }
}

std::shared_ptr<Image> Image::load(int width, int height) {
  std::call_once(freeImageContextInitFlag,
                 []() { freeImageContext = std::make_unique<FreeImageContext>(); });
  return std::make_shared<RegularImage>(IF_RegularImage, width, height);
}

Image::Image(Image::ImageFormat format, const std::shared_ptr<File>& file)
    : format_(format), file_(file) {}

RegularImage::RegularImage(ImageFormat format, int width, int height)
    : Image(format, nullptr), width_(width), height_(height), bitMap_(nullptr),
      bitMapCopy_(nullptr), memory_(nullptr) {

  // Allocate memory
  bitMap_ = FreeImage_Allocate(width_, height_, 32);

  bool colorOrderIsRGB = FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_RGB;
  colorFormat_ = colorOrderIsRGB ? ColorFormat::RGBA : ColorFormat::BGRA;

  // Get pixel data
  pixelData_ = FreeImage_GetBits(bitMap_);
  SEQUOIA_ASSERT(pixelData_);
}

RegularImage::RegularImage(ImageFormat format, const std::shared_ptr<File>& file)
    : Image(format, file), bitMap_(nullptr), bitMapCopy_(nullptr), memory_(nullptr) {

  FREE_IMAGE_FORMAT FIFormat = GetFreeImageType(getFormat());
  SEQUOIA_ASSERT_MSG(FIFormat != FIF_UNKNOWN, "invalid image format");
  SEQUOIA_ASSERT_MSG(FreeImage_FIFSupportsReading(FIFormat),
                     "format cannot be read (plugin not loaded)");

  // Attach the binary data to a memory stream. Note that a memory buffer wrapped by FreeImage is
  // read-only so const cast is safe here.
  memory_ = FreeImage_OpenMemory(const_cast<Byte*>(file_->getData()), file_->getNumBytes());

  // Load the image from the memory stream
  bitMap_ = FreeImage_LoadFromMemory(FIFormat, memory_, 0);

  // Check if the image is stored in RGB or BGR
  bool colorOrderIsRGB = FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_RGB;

  // Assert our image is 24 or 32 bits (8 bits per channel, Red/Green/Blue/Alpha)
  int bitsPerPixel = FreeImage_GetBPP(bitMap_);
  if(bitsPerPixel < 24) {
    bitMapCopy_ = FreeImage_ConvertTo24Bits(bitMap_);
    std::swap(bitMapCopy_, bitMap_);
    colorFormat_ = colorOrderIsRGB ? ColorFormat::RGB : ColorFormat::BGR;

  } else if(bitsPerPixel == 24) {
    colorFormat_ = colorOrderIsRGB ? ColorFormat::RGB : ColorFormat::BGR;

  } else if(bitsPerPixel == 32) {
    colorFormat_ = colorOrderIsRGB ? ColorFormat::RGBA : ColorFormat::BGRA;
  }

  // Get bitmap informations
  width_ = FreeImage_GetWidth(bitMap_);
  height_ = FreeImage_GetHeight(bitMap_);

  FREE_IMAGE_TYPE imageType = FreeImage_GetImageType(bitMap_);
  FREE_IMAGE_COLOR_TYPE colorType = FreeImage_GetColorType(bitMap_);

  SEQUOIA_ASSERT_MSG(colorType == FIC_RGB || colorType == FIC_RGBALPHA, "unsupported color type");
  SEQUOIA_ASSERT_MSG(imageType == FIT_BITMAP, "unsupported image type");

  // Get pixel data
  pixelData_ = FreeImage_GetBits(bitMap_);
  SEQUOIA_ASSERT(pixelData_);
}

RegularImage::~RegularImage() {
  if(bitMap_)
    FreeImage_Unload(bitMap_);

  if(bitMapCopy_)
    FreeImage_Unload(bitMapCopy_);

  if(memory_)
    FreeImage_CloseMemory(memory_);
}

std::string RegularImage::toString() const {
  return core::format("RegularImage[\n"
                      "  file = %s\n"
                      "  pixelData = %#016x\n"
                      "  width = %i,\n"
                      "  height = %i,\n"
                      "  colorFormat = %s,\n"
                      "  numChannels = %i\n"
                      "]",
                      file_ ? file_->getPath() : "null", (std::size_t)pixelData_, width_, height_,
                      colorFormat_, getNumChannels());
}

bool RegularImage::equals(const Image* other) const noexcept {
  if(!Image::equals(other))
    return false;

  const RegularImage* thisImage = dyn_cast<RegularImage>(this);
  const RegularImage* otherImage = dyn_cast<RegularImage>(other);

  return thisImage->pixelData_ == otherImage->pixelData_ &&
         thisImage->width_ == otherImage->width_ && thisImage->height_ == otherImage->height_ &&
         thisImage->colorFormat_ == otherImage->colorFormat_ &&
         *(thisImage->file_) == *(otherImage->file_);
}

std::size_t RegularImage::hash() const noexcept {
  std::size_t seed = 0;
  core::hashCombine(seed, file_->hash(), pixelData_, width_, height_, colorFormat_);
  return seed;
}

TextureImage::TextureImage(Image::ImageFormat format, const std::shared_ptr<File>& file)
    : Image(format, file) {
  SEQUOIA_ASSERT_MSG(format == IF_DDS, "only DDS can be loaded as texture image (yet)");

  // gli::load dispatches to the correct decoder
  texture_ = std::make_unique<gli::texture>(
      gli::load(reinterpret_cast<char const*>(file_->getData()), file_->getNumBytes()));

  if(texture_->empty())
    SEQUOIA_THROW(core::Exception, "failed to load texture image of file: %s", file_->getPath());
}

TextureImage::~TextureImage() {}

std::size_t TextureImage::hash() const noexcept {
  std::size_t seed = 0;
  core::hashCombine(seed, file_->hash(), std::hash<std::unique_ptr<gli::texture>>()(texture_));
  return seed;
}

std::string TextureImage::toString() const {
  return core::format("TextureImage[\n"
                      "  file = %s,\n"
                      "  width = %i,\n"
                      "  height = %i,\n"
                      "  levels = %i\n"
                      "]",
                      file_->getPath(), texture_->extent()[0], texture_->extent()[1],
                      texture_->levels());
}

bool TextureImage::equals(const Image* other) const noexcept {
  if(!Image::equals(other))
    return false;

  const TextureImage* thisImage = dyn_cast<TextureImage>(this);
  const TextureImage* otherImage = dyn_cast<TextureImage>(other);

  return *(thisImage->texture_) == *(otherImage->texture_) &&
         *(thisImage->file_) == *(otherImage->file_);
}

int TextureImage::getWidth() const noexcept { return texture_->extent()[0]; }
int TextureImage::getHeight() const noexcept { return texture_->extent()[1]; }

} // namespace core

} // namespace sequoia
