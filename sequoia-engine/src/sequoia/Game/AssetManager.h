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

#ifndef SEQUOIA_GAME_ASSETMANAGER_H
#define SEQUOIA_GAME_ASSETMANAGER_H

#include "sequoia/Core/Platform.h"
#include "sequoia/Game/Export.h"

namespace sequoia {

namespace game {

/// @brief Load assets from disk
/// @ingroup game
class SEQUOIA_GAME_API AssetManager {
  platform::Path path_;

public:
};

} // namespace game

} // namespace sequoia

#endif
