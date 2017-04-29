//===-- sequoia/Game/WindowFactory.h ------------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#ifndef SEQUOIA_GAME_WINDOWFACTORY_H
#define SEQUOIA_GAME_WINDOWFACTORY_H

#include "sequoia/Core/Assert.h"
#include "sequoia/Game/Export.h"
#include <OGRE/OgreRenderWindow.h>
#include <memory>

namespace sequoia {

namespace game {

/// @brief Factory to create Ogre::RenderWindow
///
/// @ingroup game
class WindowFactory {
public:
  /// @brief Create a Ogre::RenderWindow, register it within the root object and return a pointer
  /// reference to the create window
  static Ogre::RenderWindow* create(const std::shared_ptr<Ogre::Root>& root);
};

} // namespace game

} // namespace sequoia

#endif
