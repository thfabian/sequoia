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

#ifndef SEQUOIA_CORE_TERMINAL_H
#define SEQUOIA_CORE_TERMINAL_H

#include "sequoia-engine/Core/Export.h"
#include <iosfwd>

namespace sequoia {

namespace core {

/// @namespace Terminal
/// @brief Various terminal manipulation methods, most notably changing the color namespace Terminal
/// @ingroup core
namespace Terminal {

/// @brief Change the terminal output color.
///
/// The Terminal color will be changed either by injecting ANSI/VT100 console color codes to
/// the provided stream (Linux/Mac) or by using the Win32 API (Windows).
///
/// @code{.cpp}
///     Terminal::Color::use(std::cout, Terminal::Color::Red);
///     std::cout <<  << "This is red!\n";
///     Terminal::Color::reset(std::cout);
///     std::cout << "This is the normal color again\n";
/// @endcode
///
/// Colored output can be globally disabled by setting the static member Color::disableColor to
/// false.
///
/// @ingroup core
class SEQUOIA_API Color {
public:
  enum Code {
    None = 0,
    White,
    Red,
    Green,
    Blue,
    Magenta,
    Yellow,
    Cyan,
    Grey,

    Bold = 0x10,
    BoldWhite = Bold | White,
    BoldRed = Bold | Red,
    BoldGreen = Bold | Green,
    BoldBlue = Bold | Blue,
    BoldMagenta = Bold | Magenta,
    BoldYellow = Bold | Yellow,
    BoldCyan = Bold | Cyan,
    BoldGrey = Bold | Grey
  };

  /// @brief Change color persistently
  static void use(std::ostream& os, Code color);

  /// @brief Reset the color
  static void reset(std::ostream& os);

  /// @brief Disable colored output (default: false)
  static bool DisableColor;

  /// @brief Get the color usually associated with files (BoldWhite on Windows/Linux and Grey on
  /// Mac OSX)
  static Color::Code getFileColor();
};

/// @brief Get the current width of the terminal
/// @ingroup core
extern SEQUOIA_API int getWidth();

} // namespace Terminal

} // namespace core

} // namespace sequoia

#endif
