//===-- sequoia/Game/RenderSubsystem.h ----------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//
//
/// @file
/// Set the rendering subsystem.
//
//===------------------------------------------------------------------------------------------===//

#ifndef SEQUOIA_GAME_RENDERSUBSYSTEM_H
#define SEQUOIA_GAME_RENDERSUBSYSTEM_H

#include "sequoia/Game/Export.h"
#include <OGRE/OgrePrerequisites.h>
#include <memory>
#include <vector>

namespace sequoia {

namespace game {

/// @brief Select the rendering subsystem
///
/// @ingroup game
class SEQUOIA_GAME_EXPORT RenderSubsystem {
public:
  RenderSubsystem(const std::shared_ptr<Ogre::Root>& root);

  /// @brief Create a new rendering system and register it within Ogre
  ///
  /// @param showDialog   Show dialog to let user choose from the available renderer
  void create(bool showDialog);

private:
  std::shared_ptr<Ogre::Root> root_;
  const Ogre::vector<Ogre::RenderSystem*>::type& renderList_;
};

} // namespace game

} // namespace sequoia

#endif
