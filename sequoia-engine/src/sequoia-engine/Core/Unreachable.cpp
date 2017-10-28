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

#include "sequoia-engine/Core/Unreachable.h"
#include <cstdio>
#include <iostream>

namespace sequoia {

namespace core {

SEQUOIA_ATTRIBUTE_NORETURN void sequoia_unreachable_internal(const char* msg, const char* file,
                                                             unsigned line) {
  std::cerr << "FATAL ERROR: UNREACHABLE executed : ";
  if(msg)
    std::cerr << "\"" << msg << "\"";
  if(file)
    std::cerr << " at " << file << ":" << line;
  std::cerr << std::endl;
  std::abort();

#ifdef SEQUOIA_BUILTIN_UNREACHABLE
  SEQUOIA_BUILTIN_UNREACHABLE;
#endif
}

} // namespace core

} // namespace sequoia
