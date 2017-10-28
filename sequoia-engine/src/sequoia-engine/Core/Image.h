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

#include "sequoia-engine/Core/Color.h"
#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/File.h"

namespace gli {
class texture;
}

namespace cv {
class Mat;
}

namespace sequoia {

namespace core {

/// @brief Image interface
///
/// To load and save images use Image::load and Image::save, respectively.
/// @ingroup corerespectivly
class SEQUOIA_API Image {
public:
  /// @brief RTTI discriminator
  enum ImageKind { IK_RegularImage, IK_TextureImage };

  /// @brief Load image from file
  ///
  /// The format of the image is deduced from `File::getType()`.
  ///
  /// @param file     File to load the image from
  /// @throws Exception   Failed to load image
  ///
  /// @threadsafe This function is thread-safe
  static std::shared_ptr<Image> load(const std::shared_ptr<File>& file);

  /// @brief Allocate an empty image of size `width x height`
  ///
  /// @param width    Height of the allocated image
  /// @param height   Wdith of the allocated image
  /// @param format   Color format of the allocated image
  ///
  /// @threadsafe This function is thread-safe
  static std::shared_ptr<Image> allocate(int width, int height, ColorFormat format);

  Image(ImageKind kind, const std::shared_ptr<File>& file);
  virtual ~Image();

  /// @brief Get the file of the image
  const std::shared_ptr<File>& getFile() const;

  /// @brief Check if image has a file
  bool hasFile() const { return file_ != nullptr; }

  /// @brief Get a unique hash of the image
  virtual std::size_t hash() const noexcept = 0;

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
  virtual bool equals(const Image* other) const noexcept { return kind_ == other->kind_; }

  /// @brief Convert to string
  virtual std::string toString() const = 0;

  /// @brief Show the image in an external window (if possible)
  ///
  /// This opens an external window and **blocks** until the window is closed or *any* key is
  /// pressed.
  ///
  /// @note This might fail if OpenCV was compiled without GUI support. This function will not throw
  /// and the error will silently be ignored i.e the function will return immediately.
  virtual void show() const = 0;

  /// @brief Get the image kind
  ImageKind getKind() const { return kind_; }

protected:
  ImageKind kind_;

  /// File of the image (if any)
  std::shared_ptr<File> file_;
};

/// @brief Hardware-independent image representation that allows direct access to the pixel data
/// @ingroup core
class SEQUOIA_API RegularImage final : public Image {
protected:
  /// Image data
  std::unique_ptr<cv::Mat> image_;

  /// Format of the color
  ColorFormat colorFormat_;

public:
  /// @brief Load image from `file`
  RegularImage(const std::shared_ptr<File>& file);

  /// @brief Allocate image of size `width x height`
  RegularImage(int width, int height, ColorFormat format);

  /// @brief Deallocate image
  virtual ~RegularImage();

  /// @brief Get the pixel data
  ///
  /// The pixel data consists of `width` scanlines of `height` pixels, with each pixel consisting
  /// of `numChannels` interleaved 8-bit components; the first pixel pointed to is top-left-most in
  /// the image. There is no padding between image scanlines or between pixels, regardless of
  /// format.
  const Byte* getPixelData() const;
  Byte* getPixelData();

  /// @brief Get number of interleaved 8-bit components of each pixel
  inline int getNumChannels() const { return colorFormatGetNumChannels(colorFormat_); }

  /// @brief Get a copy of the pixel at position `(i, j)`
  ///
  /// OpenCV stores the image as BGR(A) where `(0, 0)` is the top-left corner `(getHeight() - 1, 0)`
  /// is the bottom left corner and `(0, getWidth() - 1)` is the top right corner, respectively.
  ///
  /// @verbatim
  ///                (0, 0)          (0, getWidth() - 1)                          +-------> x (u)
  ///                       +-----+                                               |
  ///                       |     |                                               |
  ///                       +-----+                                         (v)   y
  ///  (getHeight() - 1, 0)          (getHeight() - 1, getWidth() - 1)               image(y, x)
  /// @endverbatim
  ///
  /// @note This function should be merely used for debugging purposes.
  Color at(int i, int j) const;

  /// @brief Get the color format
  inline ColorFormat getColorFormat() const { return colorFormat_; }

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

  /// @brief Image::show
  virtual void show() const override;

  /// @brief Get the OpenCV matrix of the image
  const cv::Mat& getMat() const;
  cv::Mat& getMat();

  static bool classof(const Image* image) { return image->getKind() == Image::IK_RegularImage; }
};

/// @brief Texture image formats suchs as DDS
/// @ingroup core
class SEQUOIA_API TextureImage final : public Image {
protected:
  /// Image data
  std::unique_ptr<gli::texture> image_;

public:
  /// @brief Load image from `file`
  TextureImage(const std::shared_ptr<File>& file);

  /// @brief Deallocate image
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

  /// @brief Image::show
  virtual void show() const override;

  /// @brief Get the gli texture of the image
  const gli::texture& getTexture() const;
  gli::texture& getTexture();

  static bool classof(const Image* image) { return image->getKind() == Image::IK_TextureImage; }
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
