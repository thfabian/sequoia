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

#ifndef SEQUOIA_CORE_COLOR_H
#define SEQUOIA_CORE_COLOR_H

#include "sequoia/Core/Assert.h"
#include "sequoia/Core/Byte.h"
#include "sequoia/Core/Export.h"
#include <cstdint>
#include <iosfwd>
#include <utility>

namespace sequoia {

namespace core {

// Encode the number of used channels in the highest 2 bits of an 8 bit field
#define SEQUOIA_COLOR_SET_NUM_CHANNELS(NumChannels) ((std::uint8_t(NumChannels) - 1) << 6)
#define SEQUOIA_COLOR_GET_NUM_CHANNELS(Format) (((std::uint8_t(Format) & 0xC0) >> 6) + 1)

/// @brief Format of the Color
///
/// The highest two bits encode the number of required channels.
///
/// @ingroup core
enum class ColorFormat : std::uint8_t {
  RGB = SEQUOIA_COLOR_SET_NUM_CHANNELS(3) + 1,  ///< red, green, blue
  BGR = SEQUOIA_COLOR_SET_NUM_CHANNELS(3) + 2,  ///< blue, green, red
  RGBA = SEQUOIA_COLOR_SET_NUM_CHANNELS(4) + 3, ///< red, green, blue, alpha
  BGRA = SEQUOIA_COLOR_SET_NUM_CHANNELS(4) + 4, ///< blue, green, red, alpha
};

/// @brief Get the number of channels from the given `format`
/// @ingroup core
inline int colorFormatGetNumChannels(ColorFormat format) noexcept {
  return SEQUOIA_COLOR_GET_NUM_CHANNELS(format);
}

SEQUOIA_API extern std::ostream& operator<<(std::ostream& os, ColorFormat format);

#undef SEQUOIA_COLOR_SET_NUM_CHANNELS
#undef SEQUOIA_COLOR_GET_NUM_CHANNELS

/// @brief Read-only access to a color
///
/// This class is mostly for convenience and should not be used in performance critical parts.
///
/// @ingroup core
class Color {
  Byte data_[4];             ///< 32-bit data
  const ColorFormat format_; ///< Format of the color (also stores the number of used channels)

public:
  /// @brief Construct from pixel data (copies `NumChannels` Bytes starting at `data`)
  Color(ColorFormat format, const Byte* data) : format_(format) {
    for(int i = 0; i < getNumChannels(); ++i)
      this->data_[i] = data[i];
    for(int i = getNumChannels(); i < 4; ++i)
      this->data_[i] = 0;
  }
  Color(ColorFormat format, Byte* data) : Color(format, static_cast<const Byte*>(data)) {}

  /// @brief Construct from Byte array (copies `NumChannels` Bytes from `data`)
  template <int N>
  Color(ColorFormat format, Byte (&data)[N]) : Color(format, static_cast<Byte*>(data)) {}

  template <int N>
  Color(ColorFormat format, const Byte (&data)[N])
      : Color(format, static_cast<const Byte*>(data)) {}

  /// @brief Construct with integral constants
  template <class... Args>
  Color(ColorFormat format, Args&&... args) : format_(format) {
    setChannel(std::forward<Args>(args)...);
  }

  /// @brief Set the channels
  template <class... Args>
  inline void setChannel(Args&&... args) {
    setChannelImpl<0>(std::forward<Args>(args)...);
  }

  /// @brief Copy constructor
  Color(const Color&) = default;

  /// @brief Move constructor
  Color(Color&&) = default;

  /// @brief Copy assignment
  Color& operator=(const Color&) = default;

  /// @brief Move assignment
  Color& operator=(Color&&) = default;

  /// @brief Get the number of used channels
  inline std::uint8_t getNumChannels() const noexcept {
    return colorFormatGetNumChannels(format_);
  }

  /// @brief Get format
  inline ColorFormat getFormat() const noexcept { return format_; }

  /// @brief Get the value of the `N-th` channel (or 0 if the channel is unused)
  inline Byte& operator[](int N) { return data_[N]; }
  inline const Byte& operator[](int N) const { return data_[N]; }

  /// @name Check if `this` is equal to `other`
  ///
  /// This checks if red, green, blue and alpha are equal ignoring the format.
  bool operator==(const Color& other) const noexcept {
    return r() == other.r() && g() == other.g() && b() == other.b() && a() == other.a();
  }

  /// @name Check if `this` is not equal to `other`
  ///
  /// This checks if red, green, blue and alpha are equal ignoring the format.
  bool operator!=(const Color& other) const noexcept { return !(*this == other); }

  /// @brief Get red value
  Byte r() const noexcept {
    switch(format_) {
    case ColorFormat::RGB:
    case ColorFormat::RGBA:
      return data_[0];
    case ColorFormat::BGR:
    case ColorFormat::BGRA:
      return data_[2];
    default:
      return 0;
    }
  }

  /// @brief Get green value
  Byte g() const noexcept { return data_[1]; }

  /// @brief Get blue value
  Byte b() const noexcept {
    switch(format_) {
    case ColorFormat::RGB:
    case ColorFormat::RGBA:
      return data_[2];
    case ColorFormat::BGR:
    case ColorFormat::BGRA:
      return data_[0];
    default:
      return 0;
    }
  }

  /// @brief Get alpha value
  Byte a() const noexcept { return data_[3]; }

private:
  template <int Idx, class T, class... Args>
  inline void setChannelImpl(T&& arg, Args&&... args) {
    data_[Idx] = arg;
    setChannelImpl<Idx + 1>(args...);
  }

  template <int Idx, class T>
  inline void setChannelImpl(T&& arg) {
    data_[Idx] = arg;
    for(int i = Idx + 1; i < 4; ++i)
      data_[i] = 0;
  }
};

SEQUOIA_API extern std::ostream& operator<<(std::ostream& os, const Color& color);

/// @brief Make a color of the given format
template <ColorFormat Format, typename... Args>
Color makeColor(Args&&... args) {
  return Color(Format, std::forward<Args>(args)...);
}

/// @brief Make a 24-bit RGB color
template <typename... Args>
Color makeColorRGB(Args&&... args) {
  return makeColor<ColorFormat::RGB>(std::forward<Args>(args)...);
}

/// @brief Make a 24-bit BGR color
template <typename... Args>
Color makeColorBGR(Args&&... args) {
  return makeColor<ColorFormat::BGR>(std::forward<Args>(args)...);
}

/// @brief Make a 32-bit RGBA color
template <typename... Args>
Color makeColorRGBA(Args&&... args) {
  return makeColor<ColorFormat::RGBA>(std::forward<Args>(args)...);
}

/// @brief Make a 32-bit BGRA color
template <typename... Args>
Color makeColorBGRA(Args&&... args) {
  return makeColor<ColorFormat::BGRA>(std::forward<Args>(args)...);
}

} // namespace core

using Color = core::Color;

} // namespace sequoia

#endif
