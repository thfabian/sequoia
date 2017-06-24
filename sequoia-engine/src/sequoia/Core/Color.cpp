//===--------------------------------------------------------------------------------*- C++ -*-===//
//                         _____                        _
//                        / ____|                      (_)
//                       | (___   ___  __ _ _   _  ___  _  __ _
//                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
//                        ____) |  __/ (_| | |_| | (_) | | (_| |
//                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine
//                                       | |
//                                       |_|
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Core/Color.h"
#include <ostream>

namespace sequoia {

namespace core {

std::ostream& operator<<(std::ostream& os, ColorFormat format) {
  switch(format) {
  case ColorFormat::G:
    return (os << "G");
  case ColorFormat::GA:
    return (os << "GA");
  case ColorFormat::RGB:
    return (os << "RGB");
  case ColorFormat::RGBA:
    return (os << "RGBA");
  }
}

namespace {

template <class ColorT>
std::ostream& toStream(std::ostream& os, const ColorT& color) {
  os << "[ ";
  for(int i = 0; i < color.getNumChannels(); ++i)
    os << int(color[i]) << (i != (color.getNumChannels() - 1) ? ", " : "");
  os << " ]";
  return os;
}

} // anonymous namespace

} // namespace core

std::ostream& operator<<(std::ostream& os, const ColorRGBA& color) {
  return core::toStream(os, color);
}

std::ostream& operator<<(std::ostream& os, const ColorRGB& color) {
  return core::toStream(os, color);
}

std::ostream& operator<<(std::ostream& os, const ColorGA& color) {
  return core::toStream(os, color);
}

std::ostream& operator<<(std::ostream& os, const ColorG& color) {
  return core::toStream(os, color);
}

} // namespace sequoia
