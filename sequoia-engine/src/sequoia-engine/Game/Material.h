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

#ifndef SEQUOIA_ENGINE_GAME_MATERIAL_H
#define SEQUOIA_ENGINE_GAME_MATERIAL_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/Hash.h"
#include "sequoia-engine/Core/NonCopyable.h"
#include "sequoia-engine/Render/RenderFwd.h"
#include "sequoia-engine/Render/Texture.h"
#include <memory>
#include <string>

namespace sequoia {

namespace game {

/// @brief Material description of a Shape
/// @ingroup game
class SEQUOIA_API Material : public NonCopyable {
public:
  /// @brief Create the material
  ///
  /// @note This should never be called manually, use `ShapeManager::load` instead.
  Material();

  /// @brief Convert to string
  std::string toString() const;
};

} // namespace game

} // namespace sequoia

#endif
