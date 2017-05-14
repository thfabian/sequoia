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

#ifndef SEQUOIA_UNITTEST_RESSOURCEPATH_H
#define SEQUOIA_UNITTEST_RESSOURCEPATH_H

#include "sequoia/Core/Platform.h"
#include "sequoia/Unittest/Export.h"

namespace sequoia {

namespace unittest {

/// @brief Get the full path of the ressource file specified by the `path` relative to the ressource
/// root (i.e `Environment::getRessourcePath()`)
///
/// @param path   Path relative to the ressource root
///
/// @ingroup unittest
extern SEQUOIA_UNITTEST_API platform::String resolveRessourcePath(const char* path);

} // namespace unittest

} // namespace sequoia

#endif
