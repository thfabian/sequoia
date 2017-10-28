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

#include "sequoia-engine/Core/Color.h"
#include "sequoia-engine/Core/Unreachable.h"
#include <ostream>

namespace sequoia {

namespace core {

std::ostream& operator<<(std::ostream& os, ColorFormat format) {
  switch(format) {
  case ColorFormat::RGB:
    return (os << "RGB");
  case ColorFormat::BGR:
    return (os << "BGR");
  case ColorFormat::RGBA:
    return (os << "RGBA");
  case ColorFormat::BGRA:
    return (os << "BGRA");
  default:
    sequoia_unreachable("invalid format");
  }
}

std::ostream& operator<<(std::ostream& os, const Color& color) {
  os << "[ ";
  os << static_cast<int>(color[0]) << ", ";
  os << static_cast<int>(color[1]) << ", ";
  os << static_cast<int>(color[2]);

  if(color.getNumChannels() > 3)
    os << ", " << static_cast<int>(color[3]);

  os << " ] (" << color.getFormat() << ")";
  return os;
}

const Color Color::White = Color(ColorFormat::RGBA, 255, 255, 255, 255);
const Color Color::Black = Color(ColorFormat::RGBA, 0, 0, 0, 255);
const Color Color::Red = Color(ColorFormat::RGBA, 255, 0, 0, 255);
const Color Color::Green = Color(ColorFormat::RGBA, 0, 255, 0, 255);
const Color Color::Blue = Color(ColorFormat::RGBA, 0, 0, 255, 255);

} // namespace core

} // namespace sequoia
