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

#ifndef SEQUOIA_GAME_KEYMAP_H
#define SEQUOIA_GAME_KEYMAP_H

#include "sequoia/Game/Export.h"
#include "sequoia/Render/Input.h"
#include <memory>
#include <string>

namespace sequoia {

namespace game {

/// @brief This class abstracts the key mapping.
///
/// To allow remapping of keys at runtime, you should use this abstraction. For example, you might
/// want to move your character forward if the `W` key was pressed. Yet, someone else might want to
/// move the character with `E`. If your class is hardcoded on the key `W`, you would need to
/// recompile the entire application. This class provides a remedy for this problem, it allows to
/// bind the key to the action dynamically at runtime.
///
/// For example, instead of writing:
///
/// @code{.cpp}
/// void handleEvent(const KeyboardEvent& event) {
///  if(event.Key == render::Key_W) {
///    moveForward();
///  }
/// }
/// @endcode
///
/// you should use `Keymap`. This means, your character object should have a member like
///
/// @code{.cpp}
/// std::shared_ptr<Keymap> forwardKeymap;
/// @endcode
///
/// which is initialized from a global Keymapping repository (so that it can be updated). And the
/// event will be handled the following:
///
/// @code{.cpp}
/// void handleEvent(const KeyboardEvent& event) {
///  if(forwardKeymap(event)) {
///    moveForward();
///  }
/// }
/// @endcode
///
/// @ingroup game
class SEQUOIA_API Keymap {

  /// Associated key (if any)
  render::KeyboardKey key_ = render::Key_Invalid;

  /// Associated mouse (if any)
  render::MouseButton button_ = render::MouseButton_Invalid;

  /// Needed modifier key (if any)
  render::KeyModifier mod_ = render::Mod_NoModifier;

public:
  /// @brief Initialize with a `key`
  Keymap(render::KeyboardKey key, render::KeyModifier mod = render::Mod_NoModifier)
      : key_(key), button_(render::MouseButton_Invalid), mod_(mod) {}

  /// @brief Initialize with a mouse `button`
  Keymap(render::MouseButton button, render::KeyModifier mod = render::Mod_NoModifier)
      : key_(render::Key_Invalid), button_(button), mod_(mod) {}

  Keymap(const Keymap&) = default;
  Keymap(Keymap&&) = default;

  Keymap& operator=(const Keymap&) = default;
  Keymap& operator=(Keymap&&) = default;

  /// @brief Handle a MouseButtonEvent
  ///
  /// @param event  Event to handle
  /// @returns `true` if the event conatined the requested key, `false` otherwise
  inline bool handle(const render::KeyboardEvent& event) const noexcept {
    if(key_ == render::Key_Invalid)
      return false;

    if(mod_ == render::Mod_NoModifier)
      return event.Key == key_;
    else
      return event.mod(mod_) && event.Key == key_;
  }

  /// @brief Handle a KeyboardEvent
  ///
  /// @param event  Event to handle
  /// @returns `true` if the event conatined the requested mouse button, `false` otherwise
  inline bool handle(const render::MouseButtonEvent& event) const noexcept {
    if(button_ == render::MouseButton_Invalid)
      return false;

    if(mod_ == render::Mod_NoModifier)
      return event.Button == button_;
    else
      return event.mod(mod_) && event.Button == button_;
  }

  /// @brief Get the registered `button`
  render::MouseButton getButton() const noexcept { return button_; }

  /// @brief Change the mouse `button`
  void setButton(render::MouseButton button) noexcept { button_ = button; }

  /// @brief Get the registered `key`
  render::KeyboardKey getKey() const noexcept { return key_; }

  /// @brief Change the `key`
  void setKey(render::KeyboardKey key) noexcept { key_ = key; }

  /// @brief Get the modifier key
  render::KeyModifier getMod() const noexcept { return mod_; }

  /// @brief Change the modifier key
  void setMod(render::KeyModifier mod) noexcept { mod_ = mod; }

  /// @brief Convert to string
  std::string toString() const;

  /// @brief Create a default key binding
  template <class T>
  static std::shared_ptr<Keymap> makeDefault(T key,
                                             render::KeyModifier mod = render::Mod_NoModifier) {
    return std::make_shared<Keymap>(key, mod);
  }
};

} // namespace game

} // namespace sequoia

#endif
