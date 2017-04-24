//===--------------------------------------------------------------------------------*- C++ -*-===//
//                         _____                        _       
//                        / ____|                      (_)      
//                       | (___   ___  __ _ _   _  ___  _  __ _ 
//                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
//                        ____) |  __/ (_| | |_| | (_) | | (_| |
//                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine
//                                       | |                    
//                                       |_| 
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#ifndef SEQUOIA_CORE_FORMAT_H
#define SEQUOIA_CORE_FORMAT_H

#include "sequoia/Core/Assert.h"
#include "sequoia/Core/NString.h"
#include <boost/format.hpp>

namespace sequoia {

namespace core {

/// @brief printf-like formatting of a string
///
/// @param fmt    Format string
/// @param args   Variadic sequence used as arguments
///
/// @ingroup core
/// @{
template <typename... Args>
std::string format(const char* fmt, Args&&... args) {
  boost::format f(fmt);

#ifndef NDEBUG
  try {
#endif

    int unroll[]{0, (f % std::forward<Args>(args), 0)...};
    static_cast<void>(unroll);

#ifndef NDEBUG
  } catch(boost::io::format_error& error) {
    ErrorHandler::getSingleton().warning(std::string("unhandled 'boost::io::format_error': ") + error.what() + " : \"" + fmt + "\"");
    SEQUOIA_ASSERT_MSG(0, "format excpetion");
  }
#endif

  return boost::str(f);
}

template <typename... Args>
std::wstring format(const wchar_t* fmt, Args&&... args) {
  boost::wformat f(fmt);

#ifndef NDEBUG
  try {
#endif

    int unroll[]{0, (f % std::forward<Args>(args), 0)...};
    static_cast<void>(unroll);

#ifndef NDEBUG
  } catch(boost::io::format_error& error) {
    ErrorHandler::getSingleton().warning(std::wstring(L"unhandled 'boost::io::format_error': ") + toWString(error.what()) + L" : \"" + fmt + L"\"");
    SEQUOIA_ASSERT_MSG(0, "format excpetion");
  }
#endif

  return boost::str(f);
}
/// @}

} // namespace core

} // namespace sequoia

#endif
