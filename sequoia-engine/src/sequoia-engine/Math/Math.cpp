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

#include "sequoia-engine/Math/Math.h"
#include "sequoia-engine/Core/Format.h"
#include <iostream>

namespace sequoia {

namespace math {

std::string Radian::toString() const {
  return core::format("Radian[\n"
                      "  rad = {}\n"
                      "]",
                      rad_);
}

std::ostream& operator<<(std::ostream& os, const Radian& rad) { return (os << rad.rad_); }

std::string Degree::toString() const {
  return core::format("Degree[\n"
                      "  deg = {}\n"
                      "]",
                      deg_);
}

std::ostream& operator<<(std::ostream& os, const Degree& deg) { return (os << deg.deg_); }

} // namespace math

} // namspace sequoia
