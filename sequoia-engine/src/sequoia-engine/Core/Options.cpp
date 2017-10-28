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

#include "sequoia-engine/Core/Options.h"
#include "sequoia-engine/Core/Format.h"
#include <iostream>

namespace sequoia {

namespace core {

void Options::reset() {
#define OPT(Structure, Name, Type, DefaultValue, CheckFun, Doc, CommandLine, CommandLineShort,     \
            CommandLineMetaVar)                                                                    \
  Structure.Name = DefaultValue;
#include "sequoia-engine/Core/Options.inc"
#undef OPT
}

void Options::dump() {
#define OPT(Structure, Name, Type, DefaultValue, CheckFun, Doc, CommandLine, CommandLineShort,     \
            CommandLineMetaVar)                                                                    \
  std::cout << core::format("%-20s", #Structure "." #Name) << std::boolalpha << DefaultValue << "\n";
#include "sequoia-engine/Core/Options.inc"
#undef OPT
}

} // namespace core

} // namespace sequoia
