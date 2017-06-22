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
///
/// @note To load images use Image::load
/// @ingroup core
class SEQUOIA_API Image {
public:
  /// @brief RTTI discriminator
  enum ImageFormat {
    IK_Unknown,
    IK_PNG,  ///< PNGImage
    IK_JPEG, ///< JPEGImage
    IK_BMP   ///< BMPImage
  };

  /// @brief Load image from file using the given format
  ///
  /// @param file     File to load the image from
  /// @param format   Initialize image of the given format (If `IK_Unknow` is provided, the format
  ///                 is deduced from from the file extension)
  ///
  /// @throws Exception   Failed to load image
  static std::shared_ptr<Image> load(const std::shared_ptr<File>& file,
                                     ImageFormat format = IK_Unknown);

  Image(ImageFormat format);
  virtual ~Image();

  /// @brief Get the pixel data
  ///
  /// The pixel data consists of `width` scanlines of `height` pixels, with each pixel consisting
  /// of `numChannels` interleaved 8-bit components; the first pixel pointed to is top-left-most in
  /// the image. There is no padding between image scanlines or between pixels, regardless of
  /// format.
  virtual const unsigned char* getPixelData() const = 0;

  /// @brief Get read-only access to the pixel at position `(i, j)` with color of type `T`
  ///
  /// @note This function should be merely used for debugging purposes.
  template <class T>
  inline T at(int i, int j) const noexcept {
    static_assert(IsColor<T>::value, "not a Color");
    return T(getPixelData() + getNumChannels() * (i * getWidth() + j));
  }

  /// @brief Get the width of the image in pixels
  virtual int getWidth() const = 0;

  /// @brief Get the height of the image in pixels
  virtual int getHeight() const = 0;

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
  virtual int getNumChannels() const = 0;

  /// @brief Get the file of the image
  virtual const std::shared_ptr<File>& getFile() const = 0;

  /// @brief Get a unique hash of the image
  virtual std::size_t hash() const noexcept = 0;

  /// @brief Get the image format
  ImageFormat getFormat() const { return format_; }

  /// @brief Convert to string
  virtual std::string toString() const = 0;

private:
  /// Loaded image format
  ImageFormat format_;
};

/// @brief Uncompressed image formats (PNG, JPEG, BMP)
/// @ingroup core
class SEQUOIA_API UncompressedImage : public Image {
protected:
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
  UncompressedImage(ImageFormat format, const std::shared_ptr<File>& file);
  virtual ~UncompressedImage();

  /// @copydoc Image::getPixelData
  virtual const unsigned char* getPixelData() const override { return pixelData_; }

  /// @copydoc Image::getWidth
  virtual int getWidth() const override { return width_; }

  /// @copydoc Image::getHeight
  virtual int getHeight() const override { return height_; }

  /// @copydoc Image::getNumChannels
  virtual int getNumChannels() const override { return numChannels_; }

  /// @copydoc Image::getFile
  virtual const std::shared_ptr<File>& getFile() const override { return file_; }

  /// @copydoc Image::toString
  virtual std::string toString() const override;

  /// @copydoc Image::getHash
  virtual std::size_t hash() const noexcept override;

  /// @brief Get the name of the image (e.g PNGImage)
  virtual const char* getName() const = 0;
};

/// @brief Portable Network Graphics (PNG) image
/// @ingroup core
class SEQUOIA_API PNGImage : public UncompressedImage {
public:
  PNGImage(const std::shared_ptr<File>& file);
  static bool classof(const Image* image) { return image->getFormat() == Image::IK_PNG; }

  virtual const char* getName() const override { return "PNGImage"; };
};

/// @brief Joint Photographic Experts Group (JPEG) image
/// @ingroup core
class SEQUOIA_API JPEGImage : public UncompressedImage {
public:
  JPEGImage(const std::shared_ptr<File>& file);
  static bool classof(const Image* image) { return image->getFormat() == Image::IK_JPEG; }

  virtual const char* getName() const override { return "JPEGImage"; };
};

/// @brief Windows Bitmap (BMP) image
/// @ingroup core
class SEQUOIA_API BMPImage : public UncompressedImage {
public:
  BMPImage(const std::shared_ptr<File>& file);
  static bool classof(const Image* image) { return image->getFormat() == Image::IK_BMP; }

  virtual const char* getName() const override { return "BMPImage"; };
};

} // namespace core

using Image = core::Image;

} // namespace sequoia

namespace std {

template <>
struct hash<sequoia::core::Image> {
  std::size_t operator()(const sequoia::core::Image& image) const { return image.hash(); }
};

template <>
struct hash<shared_ptr<sequoia::core::Image>> {
  std::size_t operator()(const shared_ptr<sequoia::core::Image>& image) const {
    return image->hash();
  }
};

} // namespace std

#endif
