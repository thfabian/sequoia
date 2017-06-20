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

#include "sequoia/Core/Format.h"
#include "sequoia/Core/StringUtil.h"
#include "sequoia/Core/Unreachable.h"
#include "sequoia/Render/Input.h"
#include <ostream>

namespace sequoia {

namespace render {

namespace {

static const char* MouseButtonStrings[] = {
    "Left", "Right", "Middle", "Button 4", "Button 5", "Button 6", "Button 7", "Button 8",
};

static std::string modToString(int mods) {
  if(mods == 0)
    return "{}";

  std::vector<KeyModifier> modvec;
  for(auto mod : {Mod_Shift, Mod_Ctrl, Mod_Alt, Mod_Super})
    if(mods & mod)
      modvec.push_back(mod);
  return core::RangeToString(", ", "{", "}")(modvec, [](KeyModifier key) {
    std::stringstream ss;
    ss << key;
    return ss.str();
  });
}

} // anonymous namespace

std::ostream& operator<<(std::ostream& os, KeyboardKey key) noexcept {
  if(key == Key_Invalid)
    return (os << "Invalid");
  
  if(key == Key_Space)
    return (os << "Space");

  if(key <= Key_GraveAccent)
    return (os << char(key));

  switch(key) {
  case Key_World1:
    return (os << "World1");
  case Key_World2:
    return (os << "World2");
  case Key_Escape:
    return (os << "Escape");
  case Key_Enter:
    return (os << "Enter");
  case Key_Tab:
    return (os << "Tab");
  case Key_Backspace:
    return (os << "Backspace");
  case Key_Insert:
    return (os << "Insert");
  case Key_Delete:
    return (os << "Delete");
  case Key_Right:
    return (os << "Right");
  case Key_Left:
    return (os << "Left");
  case Key_Down:
    return (os << "Down");
  case Key_Up:
    return (os << "Up");
  case Key_PageUp:
    return (os << "PageUp");
  case Key_PageDown:
    return (os << "PageDown");
  case Key_Home:
    return (os << "Home");
  case Key_End:
    return (os << "End");
  case Key_CapsLock:
    return (os << "CapsLock");
  case Key_ScrollLock:
    return (os << "ScrollLock");
  case Key_NumLock:
    return (os << "NumLock");
  case Key_PrintScreen:
    return (os << "PrintScreen");
  case Key_Pause:
    return (os << "Pause");
  case Key_F1:
    return (os << "F1");
  case Key_F2:
    return (os << "F2");
  case Key_F3:
    return (os << "F3");
  case Key_F4:
    return (os << "F4");
  case Key_F5:
    return (os << "F5");
  case Key_F6:
    return (os << "F6");
  case Key_F7:
    return (os << "F7");
  case Key_F8:
    return (os << "F8");
  case Key_F9:
    return (os << "F9");
  case Key_F10:
    return (os << "F10");
  case Key_F11:
    return (os << "F11");
  case Key_F12:
    return (os << "F12");
  case Key_F13:
    return (os << "F13");
  case Key_F14:
    return (os << "F14");
  case Key_F15:
    return (os << "F15");
  case Key_F16:
    return (os << "F16");
  case Key_F17:
    return (os << "F17");
  case Key_F18:
    return (os << "F18");
  case Key_F19:
    return (os << "F19");
  case Key_F20:
    return (os << "F20");
  case Key_F21:
    return (os << "F21");
  case Key_F22:
    return (os << "F22");
  case Key_F23:
    return (os << "F23");
  case Key_F24:
    return (os << "F24");
  case Key_F25:
    return (os << "F25");
  case Key_KP_0:
    return (os << "KP_0");
  case Key_KP_1:
    return (os << "KP_1");
  case Key_KP_2:
    return (os << "KP_2");
  case Key_KP_3:
    return (os << "KP_3");
  case Key_KP_4:
    return (os << "KP_4");
  case Key_KP_5:
    return (os << "KP_5");
  case Key_KP_6:
    return (os << "KP_6");
  case Key_KP_7:
    return (os << "KP_7");
  case Key_KP_8:
    return (os << "KP_8");
  case Key_KP_9:
    return (os << "KP_9");
  case Key_KP_Decimal:
    return (os << "KP_Decimal");
  case Key_KP_Divide:
    return (os << "KP_Divide");
  case Key_KP_Multiply:
    return (os << "KP_Multiply");
  case Key_KP_Subtract:
    return (os << "KP_Subtract");
  case Key_KP_Add:
    return (os << "KP_Add");
  case Key_KP_Enter:
    return (os << "KP_Enter");
  case Key_KP_Equal:
    return (os << "KP_Equal");
  case Key_LeftShift:
    return (os << "LeftShift");
  case Key_LeftControl:
    return (os << "LeftControl");
  case Key_LeftAlt:
    return (os << "LeftAlt");
  case Key_LeftSuper:
    return (os << "LeftSuper");
  case Key_RightShift:
    return (os << "RightShift");
  case Key_RightControl:
    return (os << "RightControl");
  case Key_RightAlt:
    return (os << "RightAlt");
  case Key_RightSuper:
    return (os << "RightSuper");
  case Key_Menu:
    return (os << "Menu");
  default:
    sequoia_unreachable("invalid key");
  }
  return os;
}

std::ostream& operator<<(std::ostream& os, MouseButton button) noexcept {
  if(button == MouseButton_Invalid)
    return (os << "Invalid");
  return (os << MouseButtonStrings[button]);
}

std::ostream& operator<<(std::ostream& os, KeyModifier mod) noexcept {
  if(mod == Mod_NoModifier)
    return (os << "NoModifier");
  if(mod == Mod_Shift)
    return (os << "Shift");
  if(mod == Mod_Ctrl)
    return (os << "Ctrl");
  if(mod == Mod_Alt)
    return (os << "Alt");
  if(mod == Mod_Super)
    return (os << "Super");

  sequoia_unreachable("invalid mod");
  return os;
}

std::string KeyboardEvent::toString() const {
  return core::format("KeyboardEvent[\n"
                      "  Key = %s,\n"
                      "  Action = %s,\n"
                      "  Mods = %s\n"
                      "]",
                      Key, Action, modToString(Mods));
}

std::string MouseButtonEvent::toString() const {
  return core::format("MouseButtonEvent[\n"
                      "  Button = %s,\n"
                      "  Action = %s,\n"
                      "  Mods = %s\n"
                      "]",
                      Button, Action, modToString(Mods));
}

std::string MousePositionEvent::toString() const {
  return core::format("MousePositionEvent[\n"
                      "  XPos = %i,\n"
                      "  YPos = %i,\n"
                      "  XOffset = %i,\n"
                      "  YOffset = %i\n"
                      "]",
                      XPos, YPos, XOffset, YOffset);
}

} // namespace render

} // namespace sequoia
