//===-- sequoia/Core/Format.h -------------------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#ifndef SEQUOIA_CORE_FORMAT_H
#define SEQUOIA_CORE_FORMAT_H

#include "sequoia/Core/String.h"
#include <boost/format.hpp>

#ifndef NDEBUG
#include "sequoia/Core/Exception.h"
#endif

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
    SEQUOIA_THROW(Exception, std::string(error.what()) + " [format string = \"" + fmt +
                                 "\", number of arguments = \"" + std::to_string(sizeof...(args)) +
                                 "\"]");
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
    SEQUOIA_THROW(Exception, String(error.what()).asWStr() + L" [format string = \"" + fmt +
                                 L"\", number of arguments = \"" +
                                 std::to_wstring(sizeof...(args)) + L"\"]");
  }
#endif

  return boost::str(f);
}
/// @}

} // namespace core

} // namespace sequoia

#endif
