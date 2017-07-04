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

struct FIBITMAP;
struct FIMEMORY;

namespace gli {
class texture;
}

namespace sequoia {

namespace core {

/// @brief Image interface
///
/// To load and save images use Image::load and Image::save, respectivly.
/// @ingroup core
class SEQUOIA_API Image {
public:
  /// @brief Load image from file
  ///
  /// The format of the image is deduced from `File::getType()`.
  ///
  /// @param file     File to load the image from
  /// @throws Exception   Failed to load image
  /// @threadsafe This function is thread-safe
  static std::shared_ptr<Image> load(const std::shared_ptr<File>& file);
  
  /// @brief Create an empty image of size `width x height`
  /// 
  /// To access the pixel data, cast it to a RegularImage
  /// @code{.cpp}
  ///   auto image = dyn_pointer_cast<RegularImage>(Image::load(512, 512));
  ///   image->at(32, 32); // Pixel at position (32, 32)
  /// @endcode
  static std::shared_ptr<Image> load(int width, int height);

  /// @brief RTTI discriminator
  enum ImageFormat {
    IF_Unknown = 0,
    IF_RegularImage,
    IF_PNG,  ///< PNGImage
    IF_JPEG, ///< JPEGImage
    IF_BMP,  ///< BMPImage
    IF_RegularImageLast,
    IF_TextureImage,
    IF_DDS, ///< DDSImage
    IF_TextureImageLast
  };
  
  Image(ImageFormat format, const std::shared_ptr<File>& file);
  virtual ~Image();

  /// @brief Get the file of the image
  const std::shared_ptr<File>& getFile() const;

  /// @brief Check if image has a file
  bool hasFile() const { return file_ != nullptr; }
  
  /// @brief Get a unique hash of the image
  virtual std::size_t hash() const noexcept = 0;

  /// @brief Get the image format
  ImageFormat getFormat() const { return format_; }

  /// @name Comparison
  /// @{
  bool operator==(const Image& other) const noexcept { return equals(&other); }
  bool operator!=(const Image& other) const noexcept { return !(*this == other); }
  /// @}

  /// @brief Get the width of the image in pixels
  virtual int getWidth() const noexcept = 0;

  /// @brief Get the height of the image in pixels
  virtual int getHeight() const noexcept = 0;

  /// @brief Compare for equality
  virtual bool equals(const Image* other) const noexcept { return format_ == other->format_; }

  /// @brief Convert to string
  virtual std::string toString() const = 0;

protected:
  /// Loaded image format
  ImageFormat format_;

  /// File of the image
  std::shared_ptr<File> file_;
};

/// @brief Hardware-independent image representation that allows direct access to the pixel data
/// @ingroup core
class SEQUOIA_API RegularImage final : public Image {
protected:
  /// Pixel data
  unsigned char* pixelData_;

  /// Image width in pixels (x)
  int width_;

  /// Image height in pixels (y)
  int height_;

  /// Format of the color (also encodes the number of used channels in the upper bits)
  ColorFormat colorFormat_;

  /// FreeImage bit map
  FIBITMAP *bitMap_, *bitMapCopy_;

  /// FreeImage memory stream
  FIMEMORY* memory_;

public:
  RegularImage(ImageFormat format, const std::shared_ptr<File>& file);
  RegularImage(ImageFormat format, int width, int height);
  virtual ~RegularImage();

  /// @brief Get the pixel data
  ///
  /// The pixel data consists of `width` scanlines of `height` pixels, with each pixel consisting
  /// of `numChannels` interleaved 8-bit components; the first pixel pointed to is top-left-most in
  /// the image. There is no padding between image scanlines or between pixels, regardless of
  /// format.
  const unsigned char* getPixelData() const { return pixelData_; }
  unsigned char* getPixelData() { return pixelData_; }
  
  /// @brief Get number of interleaved 8-bit components of each pixel
  inline int getNumChannels() const { return SEQUOIA_COLOR_GET_NUM_CHANNELS(colorFormat_); }

  /// @brief Get a copy of the pixel at position `(i, j)`
  ///
  /// The image is stored in `row-major` order where `(0, 0)` is the bottome left corner. Hence,
  /// `(0, image.getWidth() - 1)` is the bottom right corner and `(image.getHeight() - 1, 0)` is
  /// the top left corner.
  ///
  /// @note This function should be merely used for debugging purposes.
  inline Color at(int i, int j) const {
    return Color(colorFormat_, pixelData_ + getNumChannels() * (i * width_ + j));
  }

  /// @brief Get the color format
  inline ColorFormat getColorFormat() const { return colorFormat_; }

  /// @copydoc Image::getHash
  virtual std::size_t hash() const noexcept override;

  /// @copydoc Image::toString
  virtual std::string toString() const override;

  /// @copydoc Image::equals
  virtual bool equals(const Image* other) const noexcept override;

  /// @copydoc Image::getWidth
  virtual int getWidth() const noexcept override { return width_; }

  /// @copydoc Image::getHeight
  virtual int getHeight() const noexcept override { return height_; }

  static bool classof(const Image* image) {
    return image->getFormat() >= Image::IF_RegularImage &&
           image->getFormat() < Image::IF_RegularImageLast;
  }
};

/// @brief Texture image formats suchs as DDS
/// @ingroup core
class SEQUOIA_API TextureImage final : public Image {
protected:
  /// Texture
  std::unique_ptr<gli::texture> texture_;

public:
  TextureImage(ImageFormat format, const std::shared_ptr<File>& file);
  virtual ~TextureImage();

  /// @copydoc Image::getHash
  virtual std::size_t hash() const noexcept override;

  /// @copydoc Image::toString
  virtual std::string toString() const override;

  /// @copydoc Image::equals
  virtual bool equals(const Image* other) const noexcept override;

  /// @copydoc Image::getWidth
  virtual int getWidth() const noexcept override;

  /// @copydoc Image::getHeight
  virtual int getHeight() const noexcept override;

  /// @brief Get the texture
  const std::unique_ptr<gli::texture>& getTexture() const { return texture_; }
  std::unique_ptr<gli::texture>& getTexture() { return texture_; }

  static bool classof(const Image* image) {
    return image->getFormat() >= Image::IF_TextureImage &&
           image->getFormat() < Image::IF_TextureImageLast;
  }
};

} // namespace core

using Image = core::Image;
using RegularImage = core::RegularImage;
using TextureImage = core::TextureImage;

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
