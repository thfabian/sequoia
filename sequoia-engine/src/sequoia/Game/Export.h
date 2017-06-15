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

#ifndef SEQUOIA_GAME_EXPORT_H
#define SEQUOIA_GAME_EXPORT_H

#ifdef SEQUOIA_DOXYGEN_INVOKED
/// @defgroup game Game
/// @brief Main game infrastructure of Sequoia.

namespace sequoia {

/// @namespace game
/// @brief Namespace containing the driver library.
namespace game {}
}
#endif

#include "sequoia/Core/Compiler.h"

#if defined(SEQUOIA_SHARED_LIBRARIES) && defined(SequoiaGame_EXPORTS)
#define SEQUOIA_API SEQUOIA_API_EXPORT
#else
#define SEQUOIA_API SEQUOIA_API_IMPORT
#endif

#endif
