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

#ifndef SEQUOIA_RENDER_INPUT_H
#define SEQUOIA_RENDER_INPUT_H

#include "sequoia/Render/Export.h"
#include <iosfwd>
#include <string>

namespace sequoia {

namespace render {

class RenderTarget;

/// @brief List of avialable keys on standard keyboards
///
/// Keyboard layout taken from GLFW3.These key codes are inspired by the USB HID Usage Tables v1.12
/// (p. 53-60), but re-arranged to map to 7-bit ASCII for printable keys (function keys are put in
/// the 256+ range).
///
/// The naming of the key codes follow these rules:
///
///   - The US keyboard layout is used
///   - Names of printable alpha-numeric characters are used (e.g. "A", "R", "3", etc.)
///   - For non-alphanumeric characters, Unicode:ish names are used (e.g. "COMMA",
///     "LEFT_SQUARE_BRACKET", etc.). Note that some names do not correspond to the Unicode standard
///     (usually for brevity)
///   - Keys that lack a clear US mapping are named "WORLD_x"
///   - For non-printable keys, custom names are used (e.g. "F4", "BACKSPACE", etc.)
///
/// @ingroup render
enum KeyboardKey : int {
  Key_Invalid = -1,

  Key_Space = 32,
  Key_Apostrophe = 39,
  Key_Comma = 44,
  Key_Minus = 45,
  Key_Period = 46,
  Key_Slash = 47,
  Key_0 = 48,
  Key_1 = 49,
  Key_2 = 50,
  Key_3 = 51,
  Key_4 = 52,
  Key_5 = 53,
  Key_6 = 54,
  Key_7 = 55,
  Key_8 = 56,
  Key_9 = 57,
  Key_Semicolon = 59,
  Key_Equal = 61,
  Key_A = 65,
  Key_B = 66,
  Key_C = 67,
  Key_D = 68,
  Key_E = 69,
  Key_F = 70,
  Key_G = 71,
  Key_H = 72,
  Key_I = 73,
  Key_J = 74,
  Key_K = 75,
  Key_L = 76,
  Key_M = 77,
  Key_N = 78,
  Key_O = 79,
  Key_P = 80,
  Key_Q = 81,
  Key_R = 82,
  Key_S = 83,
  Key_T = 84,
  Key_U = 85,
  Key_V = 86,
  Key_W = 87,
  Key_X = 88,
  Key_Y = 89,
  Key_Z = 90,
  Key_LeftBracket = 91,
  Key_Backslash = 92,
  Key_RightBracket = 93,
  Key_GraveAccent = 96,
  Key_World1 = 161,
  Key_World2 = 162,

  Key_Escape = 256,
  Key_Enter = 257,
  Key_Tab = 258,
  Key_Backspace = 259,
  Key_Insert = 260,
  Key_Delete = 261,
  Key_Right = 262,
  Key_Left = 263,
  Key_Down = 264,
  Key_Up = 265,
  Key_PageUp = 266,
  Key_PageDown = 267,
  Key_Home = 268,
  Key_End = 269,
  Key_CapsLock = 280,
  Key_ScrollLock = 281,
  Key_NumLock = 282,
  Key_PrintScreen = 283,
  Key_Pause = 284,
  Key_F1 = 290,
  Key_F2 = 291,
  Key_F3 = 292,
  Key_F4 = 293,
  Key_F5 = 294,
  Key_F6 = 295,
  Key_F7 = 296,
  Key_F8 = 297,
  Key_F9 = 298,
  Key_F10 = 299,
  Key_F11 = 300,
  Key_F12 = 301,
  Key_F13 = 302,
  Key_F14 = 303,
  Key_F15 = 304,
  Key_F16 = 305,
  Key_F17 = 306,
  Key_F18 = 307,
  Key_F19 = 308,
  Key_F20 = 309,
  Key_F21 = 310,
  Key_F22 = 311,
  Key_F23 = 312,
  Key_F24 = 313,
  Key_F25 = 314,
  Key_KP_0 = 320,
  Key_KP_1 = 321,
  Key_KP_2 = 322,
  Key_KP_3 = 323,
  Key_KP_4 = 324,
  Key_KP_5 = 325,
  Key_KP_6 = 326,
  Key_KP_7 = 327,
  Key_KP_8 = 328,
  Key_KP_9 = 329,
  Key_KP_Decimal = 330,
  Key_KP_Divide = 331,
  Key_KP_Multiply = 332,
  Key_KP_Subtract = 333,
  Key_KP_Add = 334,
  Key_KP_Enter = 335,
  Key_KP_Equal = 336,
  Key_LeftShift = 340,
  Key_LeftControl = 341,
  Key_LeftAlt = 342,
  Key_LeftSuper = 343,
  Key_RightShift = 344,
  Key_RightControl = 345,
  Key_RightAlt = 346,
  Key_RightSuper = 347,
  Key_Menu = 348,
  Key_Last = Key_Menu,
};

/// @brief Stream KeyboardKey
/// @ingroup render
SEQUOIA_API extern std::ostream& operator<<(std::ostream& os, KeyboardKey key) noexcept;

/// @brief Mouse buttons
/// @ingroup render
enum MouseButton : int {
  MouseButton_Invalid = -1,

  MouseButton_1 = 0,
  MouseButton_2 = 1,
  MouseButton_3 = 2,
  MouseButton_4 = 3,
  MouseButton_5 = 4,
  MouseButton_6 = 5,
  MouseButton_7 = 6,
  MouseButton_8 = 7,
  MouseButton_Last = MouseButton_8,
  MouseButton_Left = MouseButton_1,
  MouseButton_Right = MouseButton_2,
  MouseButton_Middle = MouseButton_3,
};

/// @brief Stream MouseButton
/// @ingroup render
SEQUOIA_API extern std::ostream& operator<<(std::ostream& os, MouseButton button) noexcept;

/// @brief Keyboard modifiers
/// @ingroup render
enum KeyModifier : int {
  Mod_NoModifier = 0x0000,
  Mod_Shift = 0x0001,
  Mod_Ctrl = 0x0002,
  Mod_Alt = 0x0004,
  Mod_Super = 0x0008
};

/// @brief Stream KeyModifier
/// @ingroup render
SEQUOIA_API extern std::ostream& operator<<(std::ostream& os, KeyModifier mod) noexcept;

/// @brief Key and button actions
/// @ingroup render
enum KeyAction : int { Action_Released = 0, Action_Pressed = 1, Action_Repeat = 2 };

/// @brief Keyboard event
/// @ingroup render
struct SEQUOIA_API KeyboardEvent {

  /// The RenderTarget that recieved the event
  RenderTarget* Target;

  /// The keyboard key that was pressed or released
  KeyboardKey Key;

  /// Action of the key
  KeyAction Action;

  /// Bit field describing which modifier keys were held down
  int Mods;

  /// @brief Check if modifier key were held down
  inline bool mod(KeyModifier modifier) const noexcept { return (Mods & (1 << modifier)); }

  /// @brief Check if the key was pressed
  inline bool pressed() const noexcept { return (Action == Action_Pressed); }

  /// @brief Check if the key was repeated
  inline bool repeated() const noexcept { return (Action == Action_Repeat); }

  /// @brief Check if the key was released
  inline bool released() const noexcept { return (Action == Action_Released); }

  /// @brief Convert to string
  std::string toString() const;
};

/// @brief Listener of keyboard events
/// @ingroup render
class SEQUOIA_API KeyboardListener {
public:
  /// @brief Handle keyboard event
  virtual void keyboardEvent(const KeyboardEvent& event) = 0;
};

/// @brief Mouse button event
/// @ingroup render
struct SEQUOIA_API MouseButtonEvent {

  /// The RenderTarget that recieved the event
  RenderTarget* Target;

  /// The mouse button that was pressed or released.
  MouseButton Button;

  /// Action of the key
  KeyAction Action;

  /// Bit field describing which modifier keys were held down
  int Mods;

  /// @brief Check if modifier key were held down
  inline bool mod(KeyModifier modifier) const noexcept { return (Mods & (1 << modifier)); }

  /// @brief Check if the key was pressed
  inline bool pressed() const noexcept { return (Action == Action_Pressed); }

  /// @brief Check if the key was repeated
  inline bool repeated() const noexcept { return (Action == Action_Repeat); }

  /// @brief Check if the key was released
  inline bool released() const noexcept { return (Action == Action_Released); }

  /// @brief Convert to string
  std::string toString() const;
};

/// @brief Mouse position event
///
/// The mouse position (`XPos`, `YPos`) is measured in screen coordinates but relative to the
/// top-left corner of the window client area.
///
/// In addition, the offset, relative to the last event, is provided. This allows to construct the
/// movement of the mouse since the last event.
///
/// @verbatim
///
///           (XOffset, YOffset)
///       O------------------------> O  (Xpos, YPos)
///       ^                          ^
///       |                          |
///  Last position            Current position
///
/// @endverbatim
///
/// @ingroup render
struct SEQUOIA_API MousePositionEvent {

  /// The RenderTarget that recieved the event
  RenderTarget* Target;

  /// X-position measured in screen coordinates but relative to the top-left corner
  int XPos;

  /// Y-position measured in screen coordinates but relative to the top-left corner
  int YPos;

  /// Offset in the X-position in screeen coordiantes since the last event
  int XOffset;

  /// Offset in the X-position in screeen coordiantes since the last event
  int YOffset;

  /// @brief Convert to string
  std::string toString() const;
};

/// @brief Listener of mouse events
/// @ingroup render
class SEQUOIA_API MouseListener {
public:
  /// @brief Handle mouse button event
  virtual void mouseButtonEvent(const MouseButtonEvent& event) = 0;

  /// @brief Handle mouse position event
  virtual void mousePositionEvent(const MousePositionEvent& event) = 0;
};

} // namespace render

using render::KeyboardKey;
using render::MouseButton;
using render::KeyModifier;
using render::KeyAction;

using MouseButtonEvent = render::MouseButtonEvent;
using MousePositionEvent = render::MousePositionEvent;
using MouseListener = render::MouseListener;

using KeyboardEvent = render::KeyboardEvent;
using KeyListener = render::KeyboardListener;

} // namespace sequoia

#endif
