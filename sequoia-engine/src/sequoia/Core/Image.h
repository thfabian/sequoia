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
    IF_Unknown = 0,
    IF_UncompressedImage,
    IF_PNG,  ///< PNGImage
    IF_JPEG, ///< JPEGImage
    IF_BMP,  ///< BMPImage
    IF_UncompressedImageLast
  };

  /// @brief Load image from file using the given format
  ///
  /// @param file     File to load the image from
  /// @param format   Initialize image of the given format (If `IK_Unknow` is provided, the format
  ///                 is deduced from from the file extension)
  ///
  /// @throws Exception   Failed to load image
  static std::shared_ptr<Image> load(const std::shared_ptr<File>& file,
                                     ImageFormat format = IF_Unknown);

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
  virtual int getWidth() const noexcept = 0;

  /// @brief Get the height of the image in pixels
  virtual int getHeight() const noexcept = 0;

  /// @brief Get number of interleaved 8-bit components of each pixel
  virtual int getNumChannels() const noexcept = 0;

  /// @brief Get the color format
  virtual ColorFormat getColorFormat() const noexcept = 0;

  /// @brief Get the file of the image
  virtual const std::shared_ptr<File>& getFile() const = 0;

  /// @brief Get a unique hash of the image
  virtual std::size_t hash() const noexcept = 0;

  /// @brief Get the image format
  ImageFormat getFormat() const { return format_; }

  /// @name Comparison
  /// @{
  bool operator==(const Image& other) const noexcept { return equals(&other); }
  bool operator!=(const Image& other) const noexcept { return !(*this == other); }
  /// @}

  /// @brief Compare for equality
  virtual bool equals(const Image* other) const noexcept { return format_ == other->format_; }

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

  /// Format of the color (also encodes the number of used channels in the upper bits)
  ColorFormat colorFormat_;

public:
  UncompressedImage(ImageFormat format, const std::shared_ptr<File>& file);
  virtual ~UncompressedImage();

  /// @copydoc Image::getPixelData
  virtual const unsigned char* getPixelData() const override final { return pixelData_; }

  /// @copydoc Image::getWidth
  virtual int getWidth() const noexcept override final { return width_; }

  /// @copydoc Image::getHeight
  virtual int getHeight() const noexcept override final { return height_; }

  /// @copydoc Image::getNumChannels
  virtual int getNumChannels() const noexcept override final {
    return SEQUOIA_COLOR_GET_NUM_CHANNELS(colorFormat_);
  }

  /// @copydoc Image::getFile
  virtual const std::shared_ptr<File>& getFile() const override final { return file_; }

  /// @copydoc Image::getColorFormat
  virtual ColorFormat getColorFormat() const noexcept override final { return colorFormat_; }

  /// @copydoc Image::getHash
  virtual std::size_t hash() const noexcept override final;

  /// @copydoc Image::toString
  virtual std::string toString() const override final;

  /// @copydoc Image::equals
  virtual bool equals(const Image* other) const noexcept override;

  static bool classof(const Image* image) {
    return image->getFormat() >= Image::IF_UncompressedImage &&
           image->getFormat() < Image::IF_UncompressedImageLast;
  }

protected:
  virtual const char* getName() const = 0;
};

/// @brief Portable Network Graphics (PNG) image
/// @ingroup core
class SEQUOIA_API PNGImage final : public UncompressedImage {
public:
  PNGImage(const std::shared_ptr<File>& file);
  static bool classof(const Image* image) { return image->getFormat() == Image::IF_PNG; }

protected:
  virtual const char* getName() const override { return "PNGImage"; };
};

/// @brief Joint Photographic Experts Group (JPEG) image
/// @ingroup core
class SEQUOIA_API JPEGImage final : public UncompressedImage {
public:
  JPEGImage(const std::shared_ptr<File>& file);
  static bool classof(const Image* image) { return image->getFormat() == Image::IF_JPEG; }

protected:
  virtual const char* getName() const override { return "JPEGImage"; };
};

/// @brief Windows Bitmap (BMP) image
/// @ingroup core
class SEQUOIA_API BMPImage final : public UncompressedImage {
public:
  BMPImage(const std::shared_ptr<File>& file);
  static bool classof(const Image* image) { return image->getFormat() == Image::IF_BMP; }

protected:
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
