//===-- sequoia/Game/Export.h -------------------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#ifndef SEQUOIA_GAME_EXPORT_H
#define SEQUOIA_GAME_EXPORT_H

#include "sequoia/Core/Config.h"

#ifdef SEQUOIA_DOXYGEN_INVOKED
/// @defgroup game Game
/// @brief Main gaming infrastructure of Sequoia.

namespace sequoia {
/// @namespace game
/// @brief Namespace containing the main game library.
namespace game {}
}
#endif

#if defined(SEQUOIA_ON_WIN32)
#if defined(SEQUOIA_SHARED_LIBRARIES) && defined(SequoiaGame_EXPORTS)
#define SEQUOIA_GAME_EXPORT __declspec(dllexport)
#else
#define SEQUOIA_GAME_EXPORT __declspec(dllimport)
#endif
#else
#define SEQUOIA_GAME_EXPORT
#endif

#endif
