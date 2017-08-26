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
#include <gli/gli.hpp>
#include <memory>
#include <opencv2/opencv.hpp>

#include <iostream>

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
  case FileType::Tiff:
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

RegularImage::RegularImage(const std::shared_ptr<File>& file) : Image(IK_RegularImage, file) {
  try {
    image_ = std::make_unique<cv::Mat>(cv::imdecode(
        cv::Mat(1, file->getNumBytes(), CV_8UC1, (void*)file->getData()), CV_LOAD_IMAGE_COLOR));
  } catch(cv::Exception& e) {
    SEQUOIA_THROW(core::Exception, "failed to load image from file: %s: %s", file_->getPath(),
                  e.what());
  }

  if(image_->empty())
    SEQUOIA_THROW(core::Exception, "failed to load image from file: %s", file_->getPath());

  // OpenCV images are *always* BGR(A)
  switch(image_->channels()) {
  case 3:
    colorFormat_ = ColorFormat::BGR;
    break;
  case 4:
    colorFormat_ = ColorFormat::BGRA;
    break;
  default:
    sequoia_unreachable("invalid color format");
  }
}

RegularImage::~RegularImage() {}

const Byte* RegularImage::getPixelData() const { return image_->ptr(); }

Byte* RegularImage::getPixelData() { return image_->ptr(); }

Color RegularImage::at(int i, int j) const {
  return Color(colorFormat_, image_->ptr() + image_->channels() * (i * image_->rows + j));
}

int RegularImage::getWidth() const noexcept { return image_->cols; }

int RegularImage::getHeight() const noexcept { return image_->rows; }

const cv::Mat& RegularImage::getMat() const { return *image_; }

cv::Mat& RegularImage::getMat() { return *image_; }

void RegularImage::show() const {
  const char* winName = hasFile() ? getFile()->getPath().c_str() : "unknown";

  try {
    cv::namedWindow(winName);
    cv::imshow(winName, *image_);
    cv::waitKey(0);
  } catch(cv::Exception& e) {
    LOG(ERROR) << "Failed to to show image \"" << winName << "\": " << e.what();
  }
}

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
    SEQUOIA_THROW(core::Exception, "failed to load texture image from file: %s", file_->getPath());
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
                      file_->getPath(), image_->extent()[0], image_->extent()[1], image_->levels());
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

void TextureImage::show() const {
  LOG(ERROR) << "Failed to to show image \"" << file_->getPath() << "\": DDS images can't be shown";
}

const gli::texture& TextureImage::getTexture() const { return *image_; }

gli::texture& TextureImage::getTexture() { return *image_; }

} // namespace core

} // namespace sequoia
