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

#include "sequoia-engine/Core/Terminal.h"
#include "sequoia-engine/Core/Compiler.h"
#include "sequoia-engine/Core/Unreachable.h"
#include <iostream>

#ifdef SEQUOIA_ON_WIN32
#include <windows.h>
#elif defined(SEQUOIA_ON_UNIX)
#include <sys/ioctl.h>
#include <unistd.h>
#endif

namespace sequoia {

namespace core {

using namespace Terminal;

int Terminal::getWidth() {
  int width = 80; // default
#ifdef SEQUOIA_ON_WIN32
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
  width = (int)(csbi.srWindow.Right - csbi.srWindow.Left + 1);
#elif defined(SEQUOIA_ON_UNIX)
  struct winsize ws;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
  width = (int)ws.ws_col;
#endif
  return (width <= 80 || width >= 256 ? 80 : width);
}

namespace internal {

struct ColorImpl {
  virtual ~ColorImpl() {}
  virtual void use(std::ostream& os, Color::Code colorCode) = 0;
};

// Empty implementation
struct ColorImplEmpty : public ColorImpl {
  virtual ~ColorImplEmpty() {}
  virtual void use(std::ostream& os, Color::Code colorCode) override {}
};

#ifdef SEQUOIA_ON_WIN32

// Windows implementation
struct ColorImplWin32 : public ColorImpl {
  enum HandleKind { HK_Stdout, HK_Stderr };

  struct ConsoleInfo {
    HANDLE Handle;
    WORD ForeAtrr;
    WORD BackAttr;
  };

  ColorImplWin32() {
    consoleInfo[HK_Stdout] = ConsoleInfo();
    consoleInfo[HK_Stdout].Handle = GetStdHandle(STD_OUTPUT_HANDLE);
    consoleInfo[HK_Stderr] = ConsoleInfo();
    consoleInfo[HK_Stderr].Handle = GetStdHandle(STD_ERROR_HANDLE);

    auto queryAttr = [&](HandleKind handle) {
      CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
      GetConsoleScreenBufferInfo(consoleInfo[handle].Handle, &csbiInfo);
      consoleInfo[handle].ForeAtrr =
          csbiInfo.wAttributes &
          ~(BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
      consoleInfo[handle].BackAttr =
          csbiInfo.wAttributes &
          ~(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    };

    queryAttr(HK_Stdout);
    queryAttr(HK_Stderr);
  }

  virtual ~ColorImplWin32() {}

  virtual void use(std::ostream& os, Color::Code colorCode) override {
    HandleKind handle = HK_Stdout;
    if(&os == &std::cout || &os == &std::clog)
      handle = HK_Stdout;
    else if(&os == &std::cerr)
      handle = HK_Stderr;
    else
      return;

    auto setAttr = [&](WORD textAttr) -> void {
      SetConsoleTextAttribute(consoleInfo[handle].Handle, textAttr | consoleInfo[handle].BackAttr);
    };

    switch(colorCode) {
    case Color::None:
      setAttr(consoleInfo[handle].ForeAtrr);
      break;
    case Color::White:
      setAttr(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
      break;
    case Color::Red:
      setAttr(FOREGROUND_RED);
      break;
    case Color::Blue:
      setAttr(FOREGROUND_BLUE);
      break;
    case Color::Green:
      setAttr(FOREGROUND_GREEN);
      break;
    case Color::Magenta:
      setAttr(FOREGROUND_RED | FOREGROUND_BLUE);
      break;
    case Color::Yellow:
      setAttr(FOREGROUND_RED | FOREGROUND_GREEN);
      break;
    case Color::Cyan:
      setAttr(FOREGROUND_BLUE | FOREGROUND_GREEN);
      break;
    case Color::Grey:
      setAttr(FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
      break;
    case Color::BoldWhite:
      setAttr(FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
      break;
    case Color::BoldRed:
      setAttr(FOREGROUND_INTENSITY | FOREGROUND_RED);
      break;
    case Color::BoldBlue:
      setAttr(FOREGROUND_INTENSITY | FOREGROUND_BLUE);
      break;
    case Color::BoldGreen:
      setAttr(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
      break;
    case Color::BoldMagenta:
      setAttr(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
      break;
    case Color::BoldYellow:
      setAttr(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
      break;
    case Color::BoldCyan:
      setAttr(FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
      break;
    case Color::BoldGrey:
      setAttr(FOREGROUND_INTENSITY);
      break;
    default: // Color::Bold
      sequoia_unreachable("invalid color");
    }
  }

private:
  ConsoleInfo consoleInfo[2];
};

#elif defined(SEQUOIA_ON_UNIX)

// UNIX implementation
struct ColorImplUnix : public ColorImpl {
  virtual ~ColorImplUnix() {}
  virtual void use(std::ostream& os, Color::Code colorCode) override {
    if(!(&os == &std::cout || &os == &std::clog || &os == &std::cerr))
      return;

    auto setAttr = [&](const char* escapeCode) -> void { os << "\033" << escapeCode; };

    switch(colorCode) {
    case Color::None:
      setAttr("[0;39m");
      break;
    case Color::White:
      setAttr("[0;37m");
      break;
    case Color::Red:
      setAttr("[0;31m");
      break;
    case Color::Green:
      setAttr("[0;32m");
      break;
    case Color::Blue:
      setAttr("[0;34m");
      break;
    case Color::Magenta:
      setAttr("[0;35m");
      break;
    case Color::Yellow:
      setAttr("[0;33m");
      break;
    case Color::Cyan:
      setAttr("[0;36m");
      break;
    case Color::Grey:
      setAttr("[1;30m");
      break;
    case Color::BoldWhite:
      setAttr("[1;37m");
      break;
    case Color::BoldRed:
      setAttr("[1;31m");
      break;
    case Color::BoldBlue:
      setAttr("[1;34m");
      break;
    case Color::BoldGreen:
      setAttr("[1;32m");
      break;
    case Color::BoldMagenta:
      setAttr("[1;35m");
      break;
    case Color::BoldYellow:
      setAttr("[1;33m");
      break;
    case Color::BoldCyan:
      setAttr("[1;36m");
      break;
    case Color::BoldGrey:
      setAttr("[0;37m");
      break;
    default: // Color::Bold
      sequoia_unreachable("invalid color");
    }
  }
};

#endif

// Factory method to get the platform specific implementation
static ColorImpl* ColorFactory() {
  if(Terminal::Color::DisableColor) {
    static ColorImplEmpty colorInstance;
    return static_cast<ColorImpl*>(&colorInstance);
  }
#ifdef SEQUOIA_ON_WIN32
  static ColorImplWin32 colorInstance;
#elif defined(SEQUOIA_ON_UNIX)
  static ColorImplUnix colorInstance;
#else
  static ColorImplEmpty colorInstance;
#endif
  return static_cast<ColorImpl*>(&colorInstance);
}

} // namespace internal

Color::Code Color::getFileColor() {
#ifdef SEQUOIA_ON_APPLE
  return Color::Grey;
#else
  return Color::BoldWhite;
#endif
}

bool Terminal::Color::DisableColor = false;

void Color::use(std::ostream& os, Color::Code colourCode) {
  static internal::ColorImpl* colorInstance = internal::ColorFactory();
  colorInstance->use(os, colourCode);
}

void Color::reset(std::ostream& os) {
  static internal::ColorImpl* colorInstance = internal::ColorFactory();
  colorInstance->use(os, Color::None);
}

} // namespace core

} // namespace sequoia
