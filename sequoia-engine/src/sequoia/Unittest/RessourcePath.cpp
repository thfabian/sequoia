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

#include "sequoia/Unittest/RessourcePath.h"
#include "sequoia/Core/UtfString.h"
#include "sequoia/Unittest/Environment.h"

namespace sequoia {

namespace unittest {

platform::String resolveRessourcePath(const char* path) {
#ifdef SEQUOIA_ON_WIN32
  return platform::Path(Environment::getSingleton().getRessourcePath() /
                        UtfString(path).toWideString())
      .native();
#else
  return platform::Path(Environment::getSingleton().getRessourcePath() / path).native();
#endif
}

} // namespace unittest

} // namespace sequoia
