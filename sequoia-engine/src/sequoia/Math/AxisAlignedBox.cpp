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

#include "sequoia/Core/Format.h"
#include "sequoia/Math/AxisAlignedBox.h"
#include <iostream>

namespace sequoia {

namespace math {

std::string AxisAlignedBox::toString() const {
  if(extent_ != EK_Finite)
    return core::format("AxisAlignedBox[\n"
                        "  extent = %s\n"
                        "]",
                        extent_ == EK_Null ? "null" : "infinite");
  else
    return core::format("AxisAlignedBox[\n"
                        "  extent = finite,\n"
                        "  minimum = %s,\n"
                        "  maximum = %s\n"
                        "]",
                        minimum_, maximum_);
}

} // namespace math

} // namespace sequoia
