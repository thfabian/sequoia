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

#ifndef SEQUOIA_ENGINE_CORE_COLOR_H
#define SEQUOIA_ENGINE_CORE_COLOR_H

#include "sequoia-engine/Core/Byte.h"
#include "sequoia-engine/Core/Export.h"
#include <algorithm>
#include <array>
#include <cstdint>
#include <iosfwd>
#include <limits>

namespace sequoia {

namespace core {

/// @brief Representation of an RGBA color
/// @ingroup core
struct Color {
  /// @brief Minumum and maximum Byte values
  /// @{
  static constexpr Byte Uint8Max = std::numeric_limits<Byte>::max();
  static constexpr Byte Uint8Min = std::numeric_limits<Byte>::min();
  /// @}

  /// Access to the color data
  union {
    Byte data[4];
    struct { Byte r, g, b, a; };
  };

  /// @brief Number of color channels
  static const int NumChannels = 4;

  /// @brief Construct from RGB(A) sequence
  Color(Byte red = 0, Byte green = 0, Byte blue = 0, Byte alpha = Color::Uint8Max) {
    this->r = red;
    this->g = green;
    this->b = blue;
    this->a = alpha;
  }

  /// @brief Construct from byte pointer
  ///
  /// The color is default construct and then `min(numChannels, Color::NumChannels)` Bytes are
  /// copied.
  Color(const Byte* dataPtr, int numChannels) : Color() { setChannels(dataPtr, numChannels); }

  /// @brief Construct from byte array `dataArray`
  ///
  /// The color is default construct and then `min(numChannels, Color::NumChannels)` Bytes are
  /// copied.
  template <int N>
  Color(Byte (&dataArray)[N]) : Color(static_cast<Byte*>(dataArray), N) {}

  /// @brief Copy constructor
  Color(const Color&) = default;

  /// @brief Move constructor
  Color(Color&&) = default;

  /// @brief Copy assignment
  Color& operator=(const Color&) = default;

  /// @brief Move assignment
  Color& operator=(Color&&) = default;

  /// @brief Get the value of the `N-th` channel (or 0 if the channel is unused)
  inline Byte& operator[](int N) { return this->data[N]; }
  inline const Byte& operator[](int N) const { return this->data[N]; }

  /// @name Check if `this` is equal to `other`.
  bool operator==(const Color& other) const noexcept {
    return this->r == other.r && this->g == other.g && this->b == other.b && this->a == other.a;
  }

  /// @name Check if `this` is not equal to `other`
  bool operator!=(const Color& other) const noexcept { return !(*this == other); }

  /// @brief Convert to 32-bit RGBA floating point
  ///
  /// This normalizes the channels to [0, 1) floating point numbers an returns them in RGBA format
  inline std::array<float, 4> toRGBA32f() const noexcept {
    return {{float(this->r) / Color::Uint8Max, float(this->g) / Color::Uint8Max,
             float(this->b) / Color::Uint8Max, float(this->a) / Color::Uint8Max}};
  }

  /// @brief Commonly used colors
  /// @{
  static const Color White;
  static const Color Black;
  static const Color Red;
  static const Color Green;
  static const Color Blue;
  /// @}

  /// @brief Set the color channel by copying `min(numChannels, Color::NumChannels)` Bytes
  inline void setChannels(const Byte* dataPtr, int numChannels) noexcept {
    const int maxNumChannels = Color::NumChannels;
    const int channelsToCopy = std::min(numChannels, maxNumChannels);
    for(int i = 0; i < channelsToCopy; ++i)
      this->data[i] = dataPtr[i];
  }
};

SEQUOIA_API extern std::ostream& operator<<(std::ostream& os, const Color& color);

} // namespace core

using Color = core::Color;

} // namespace sequoia

#endif
