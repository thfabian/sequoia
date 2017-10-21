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

#include "sequoia/Math/AxisAlignedBox.h"
#include "sequoia/Core/Format.h"

namespace sequoia {

namespace math {

std::string AxisAlignedBox::toString() const {
  if(extent_ != EK_Finite)
    return core::format("AxisAlignedBox[\n"
                        "  extent = {}\n"
                        "]",
                        extent_ == EK_Null ? "null" : "infinite");
  else
    return core::format("AxisAlignedBox[\n"
                        "  extent = finite,\n"
                        "  minimum = {},\n"
                        "  maximum = {}\n"
                        "]",
                        minimum_, maximum_);
}

} // namespace math

} // namespace sequoia
