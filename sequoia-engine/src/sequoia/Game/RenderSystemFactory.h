//===-- sequoia/Game/RenderSystemFactory.h ------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#ifndef SEQUOIA_GAME_RENDERSYSTEMFACTORY_H
#define SEQUOIA_GAME_RENDERSYSTEMFACTORY_H

#include "sequoia/Game/Export.h"
#include <OGRE/OgrePrerequisites.h>
#include <memory>
#include <string>
#include <vector>

namespace sequoia {

namespace game {

/// @brief Factory to create RenderSystems
///
/// @ingroup game
class RenderSystemFactory {
public:
  /// @brief Create a new rendering system and register it within Ogre
  ///
  /// @param root                   Root object
  /// @param showDialog             Show dialog to let user choose from the available renderer
  /// @param preferredRenderSystem  Preffered rendering system
  static Ogre::RenderSystem* create(const std::shared_ptr<Ogre::Root>& root, bool showDialog,
                                    std::string preferredRenderSystem = "");
};

} // namespace game

} // namespace sequoia

#endif
