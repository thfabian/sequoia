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

std::ostream& operator<<(std::ostream& os, const Color& color) {
  os << "[ " << static_cast<int>(color.r) << ", " << static_cast<int>(color.g) << ", "
     << static_cast<int>(color.b) << ", " << static_cast<int>(color.a) << " ]";
  return os;
}

static_assert(Color::Uint8Max == 255, "inconsistent color definitions");

const Color Color::White = Color(255, 255, 255, 255);
const Color Color::Black = Color(0, 0, 0, 255);
const Color Color::Red = Color(255, 0, 0, 255);
const Color Color::Green = Color(0, 255, 0, 255);
const Color Color::Blue = Color(0, 0, 255, 255);

} // namespace core

} // namespace sequoia
