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

#ifndef SEQUOIA_CORE_IMAGE_H
#define SEQUOIA_CORE_IMAGE_H

#include "sequoia/Core/Color.h"
#include "sequoia/Core/Export.h"
#include "sequoia/Core/File.h"

namespace sequoia {

namespace core {

/// @brief Hardware-independent image representation that allows direct access to the pixel data
/// @ingroup core
class SEQUOIA_API Image {

  /// File of the image
  std::shared_ptr<File> file_;

  /// Pixel data
  unsigned char* pixelData_;

  /// Image width in pixels (x)
  int width_;

  /// Image height in pixels (y)
  int height_;

  /// Outputs # of image components in image file
  int numChannels_;

public:
  /// @brief Initialize the image with a file and load the image
  ///
  /// @throws Exception   Failed to load image
  Image(const std::shared_ptr<File>& file);
  ~Image();

  /// @brief Get the pixel data
  ///
  /// The pixel data consists of `width` scanlines of `height` pixels, with each pixel consisting
  /// of `numChannels` interleaved 8-bit components; the first pixel pointed to is top-left-most in
  /// the image. There is no padding between image scanlines or between pixels, regardless of
  /// format.
  const unsigned char* getPixelData() const { return pixelData_; }

  /// @brief Get read-only access to the pixel at position `(i, j)` with color of type `T`
  template <class T>
  inline T at(int i, int j) const noexcept {
    static_assert(IsColor<T>::value, "not a Color");
    return T(pixelData_ + numChannels_ * (i * width_ + j));
  }

  /// @brief Get the color mode used in the pixel data

  /// @brief Get the width of the image in pixels
  int getWidth() const { return width_; }

  /// @brief Get the height of the image in pixels
  int getHeight() const { return height_; }

  /// @brief Get number of interleaved 8-bit components of each pixel
  ///
  /// An output image with N components has the following components interleaved in this order in
  /// each pixel:
  ///
  ///   |  N = #comp |   Components             |
  ///   |------------|:-------------------------|
  ///   |   1        |  grey                    |
  ///   |   2        |  grey, alpha             |
  ///   |   3        |  red, green, blue        |
  ///   |   4        |  red, green, blue, alpha |
  ///
  int getNumChannels() const { return numChannels_; }

  /// @brief Get the file of the image
  const std::shared_ptr<File>& getFile() const { return file_; }
};

} // namespace core

using Image = core::Image;

} // namespace sequoia

#endif
