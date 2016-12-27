//===-- sequoia/Game/RenderWindow.h -------------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//
//
/// @file
/// Setup the render window.
//
//===------------------------------------------------------------------------------------------===//

#ifndef SEQUOIA_GAME_RENDERWINDOW_H
#define SEQUOIA_GAME_RENDERWINDOW_H

#include "sequoia/Core/Assert.h"
#include "sequoia/Game/Export.h"
#include <OGRE/OgrePrerequisites.h>
#include <memory>

namespace sequoia {

namespace game {

/// @brief Reference to a Ogre::RenderWindow
class SEQUOIA_GAME_EXPORT RenderWindow {
public:
  RenderWindow(const std::shared_ptr<Ogre::Root>& root);

  /// @brief Create a new window and register it within Ogre
  void create();

  /// @brief Get underlying Ogre::RenderWindow
  Ogre::RenderWindow* get() noexcept {
    SEQUOIA_ASSERT(window_);
    return window_;
  }

  const Ogre::RenderWindow* get() const noexcept {
    SEQUOIA_ASSERT(window_);
    return window_;
  }

private:
  std::shared_ptr<Ogre::Root> root_;
  Ogre::RenderWindow* window_; ///< Non-owning pointer
};

} // namespace game

} // namespace sequoia

#endif
