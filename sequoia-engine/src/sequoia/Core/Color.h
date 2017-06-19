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

#include "sequoia/Core/Byte.h"
#include "sequoia/Core/Export.h"
#include <cstdint>
#include <iosfwd>
#include <utility>

namespace sequoia {

namespace core {

// Encode the number of used channels in the highest 2 bits of an 8 bit field
#define SEQUOIA_COLOR_SET_NUM_CHANNELS(NumChannels) ((NumChannels - 1) << 6)
#define SEQUOIA_COLOR_GET_NUM_CHANNELS(Format) (((Format & 0xC0) >> 6) + 1)

/// @brief Format of the Color
///
/// The highest two bits encode the number of required channels.
///
/// @ingroup core
enum ColorFormat : std::uint8_t {
  G = SEQUOIA_COLOR_SET_NUM_CHANNELS(1) + 0,    ///< grey
  GA = SEQUOIA_COLOR_SET_NUM_CHANNELS(2) + 1,   ///< grey, alpha
  RGB = SEQUOIA_COLOR_SET_NUM_CHANNELS(3) + 2,  ///< red, green, blue
  RGBA = SEQUOIA_COLOR_SET_NUM_CHANNELS(4) + 3, ///< red, green, blue, alpha
};

/// @brief Representation of a 32-bit Color in given format
/// @ingroup core
template <ColorFormat CFormat>
class Color {
public:
  /// @brief Color format
  static constexpr ColorFormat Format = CFormat;

  /// @brief Number of Channels
  static constexpr int NumChannels = SEQUOIA_COLOR_GET_NUM_CHANNELS(Format);

  /// @brief Construct from pixel data (copies `NumChannels` Bytes starting at `data`)
  Color(const Byte* data) {
    for(int i = 0; i < NumChannels; ++i)
      data_[i] = data[i];
    for(int i = NumChannels; i < 4; ++i)
      data_[i] = 0;
  }

  Color(Byte* data) : Color(static_cast<const Byte*>(data)) {}

  /// @brief Construct from Byte array (copies `NumChannels` Bytes from `data`)
  template <int N>
  Color(Byte (&data)[N]) : Color(static_cast<Byte*>(data)) {}

  template <int N>
  Color(const Byte (&data)[N]) : Color(static_cast<const Byte*>(data)) {}

  /// @brief Construct with integral constants
  template <class... Args>
  Color(Args&&... args) {
    setChannel(std::forward<Args>(args)...);
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
  constexpr int getNumChannels() const { return NumChannels; }

  /// @brief Get the value of the `N-th` channel (or 0 if the channel is unused)
  /// @{
  template <int N>
  inline Byte& getChannel() {
    return data_[N];
  }
  template <int N>
  inline const Byte& getChannel() const {
    return data_[N];
  }
  inline Byte& getChannel(int N) { return data_[N]; }
  inline const Byte& getChannel(int N) const { return data_[N]; }

  inline Byte& operator[](int N) { return data_[N]; }
  inline const Byte& operator[](int N) const { return data_[N]; }
  /// @}

  /// @name RGBA accessor
  /// @{
  inline Byte& r() { return data_[0]; }
  inline const Byte& r() const { return data_[0]; }

  inline Byte& g() { return data_[1]; }
  inline const Byte& g() const { return data_[1]; }

  inline Byte& b() { return data_[2]; }
  inline const Byte& b() const { return data_[2]; }

  inline Byte& a() { return data_[3]; }
  inline const Byte& a() const { return data_[3]; }
  /// @}

  /// @name Comparison
  /// @{
  template <ColorFormat OtherCFormat>
  bool operator==(const Color<OtherCFormat>& other) const noexcept {
    for(int i = 0; i < 4; ++i)
      if(data_[i] != other.data_[i])
        return false;
    return true;
  }

  template <ColorFormat OtherCFormat>
  bool operator!=(const Color<OtherCFormat>& other) const noexcept {
    return !(*this == other);
  }
  /// @}

  /// @brief Get the data pointer
  Byte* data() { return (&data_[0]); }
  const Byte* data() const { return (&data_[0]); }

private:
  template <class... Args>
  inline void setChannel(Args&&... args) {
    static_assert(sizeof...(args) <= NumChannels, "too many colors provided");
    setChannelImpl<0>(std::forward<Args>(args)...);
  }

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

private:
  Byte data_[4]; // 32 bit
};

#undef SEQUOIA_COLOR_SET_NUM_CHANNELS
#undef SEQUOIA_COLOR_GET_NUM_CHANNELS

} // namespace core

/// @brief RGBA Color (red, green, blue, alpha)
/// @see sequoia::core::Color
/// @ingroup core
using ColorRGBA = core::Color<core::ColorFormat::RGBA>;
SEQUOIA_API std::ostream& operator<<(std::ostream& os, const ColorRGBA& color);

/// @brief RGB Color (red, green, blue)
/// @see sequoia::core::Color
/// @ingroup core
using ColorRGB = core::Color<core::ColorFormat::RGB>;
SEQUOIA_API std::ostream& operator<<(std::ostream& os, const ColorRGB& color);

/// @brief GA Color (grey, alpha)
/// @see sequoia::core::Color
/// @ingroup core
using ColorGA = core::Color<core::ColorFormat::GA>;
SEQUOIA_API std::ostream& operator<<(std::ostream& os, const ColorGA& color);

/// @brief G Color (grey)
/// @see sequoia::core::Color
/// @ingroup core
using ColorG = core::Color<core::ColorFormat::G>;
SEQUOIA_API std::ostream& operator<<(std::ostream& os, const ColorG& color);

} // namespace sequoia

#endif
