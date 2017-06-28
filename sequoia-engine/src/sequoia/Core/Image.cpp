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
#include "sequoia/Core/Logging.h"
#include "sequoia/Core/StringSwitch.h"
#include "sequoia/Core/Unreachable.h"
#include <FreeImage/FreeImage.h>
#include <memory>
#include <mutex>

#include <iostream>

namespace sequoia {

namespace core {

namespace {

/// @brief Context of the FreeImage library
struct FreeImageContext {
  FreeImageContext() {
    FreeImage_Initialise();
    FreeImage_SetOutputMessage(FreeImageContext::ErrorHandler);
    LOG(INFO) << "Using FreeImage: " << FreeImage_GetVersion();
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
  case sequoia::core::Image::IF_PNG:
    return FIF_PNG;
  case sequoia::core::Image::IF_JPEG:
    return FIF_JPEG;
  case sequoia::core::Image::IF_BMP:
    return FIF_BMP;
  default:
    return FIF_UNKNOWN;
  }
}

} // anonymous namespace

Image::~Image() {}

std::shared_ptr<Image> Image::load(const std::shared_ptr<File>& file) {
  std::call_once(freeImageContextInitFlag,
                 []() { freeImageContext = std::make_unique<FreeImageContext>(); });

  switch(file->getType()) {
  case FileType::Png:
    return std::make_shared<PNGImage>(file);
    break;
  case FileType::Jpeg:
    return std::make_shared<JPEGImage>(file);
    break;
  case FileType::Bmp:
    return std::make_shared<BMPImage>(file);
    break;
  default:
    SEQUOIA_THROW(Exception, "invalid image format of file: \"%s\"", file->getPath());
    return nullptr;
  }
}

Image::Image(Image::ImageFormat format) : format_(format) {}

UncompressedImage::UncompressedImage(ImageFormat format, const std::shared_ptr<File>& file)
    : Image(format), file_(file), bitMap_(nullptr), bitMapConverted_(nullptr), converted_(false),
      memory_(nullptr) {

  FREE_IMAGE_FORMAT FIFormat = GetFreeImageType(getFormat());
  SEQUOIA_ASSERT_MSG(FIFormat != FIF_UNKNOWN, "invalid image format");
  SEQUOIA_ASSERT_MSG(FreeImage_FIFSupportsReading(FIFormat),
                     "format cannot be read (plugin not loaded)");

  // Attach the binary data to a memory stream. Note that a memory buffer wrapped by FreeImage is
  // read-only so const cast is safe here.
  memory_ = FreeImage_OpenMemory(const_cast<Byte*>(file_->getData()), file_->getNumBytes());

  // Load an image from the memory stream
  bitMap_ = FreeImage_LoadFromMemory(FIFormat, memory_, 0);

  // Get bitmap informations
  width_ = FreeImage_GetWidth(bitMap_);
  height_ = FreeImage_GetHeight(bitMap_);

  // Check if the image is stored in RGB or RGBA
  bool colorOrderIsRGB = FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_RGB;
  int bitsPerPixel = FreeImage_GetBPP(bitMap_);
  
  // Convert our image to 24 or 32 bits (8 bits per channel, Red/Green/Blue/Alpha)
  if(bitsPerPixel < 24) {
    bitMapConverted_ = FreeImage_ConvertTo24Bits(bitMap_);
    colorFormat_ = colorOrderIsRGB ? ColorFormat::RGB : ColorFormat::BGR;
    converted_ = true;
  } else if(bitsPerPixel == 24) {
    bitMapConverted_ = bitMap_;
    colorFormat_ = colorOrderIsRGB ? ColorFormat::RGB : ColorFormat::BGR;
  } else if(bitsPerPixel == 32) {
    bitMapConverted_ = bitMap_;
    colorFormat_ = colorOrderIsRGB ? ColorFormat::RGBA : ColorFormat::BGRA;
  }

  FREE_IMAGE_TYPE imageType = FreeImage_GetImageType(bitMapConverted_);
  FREE_IMAGE_COLOR_TYPE colorType = FreeImage_GetColorType(bitMapConverted_);

  SEQUOIA_ASSERT_MSG(colorType == FIC_RGB || colorType == FIC_RGBALPHA, "unsupported color type");
  SEQUOIA_ASSERT_MSG(imageType == FIT_BITMAP, "unsupported image type");

  // Get pixel data
  pixelData_ = FreeImage_GetBits(bitMapConverted_);
  SEQUOIA_ASSERT(pixelData_);
}

UncompressedImage::~UncompressedImage() {
  if(bitMap_)
    FreeImage_Unload(bitMap_);

  if(converted_ && bitMapConverted_)
    FreeImage_Unload(bitMapConverted_);

  if(memory_)
    FreeImage_CloseMemory(memory_);
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
