#ifndef SEQUOIA_CORE_FORMAT
#define SEQUOIA_CORE_FORMAT

#include "sequoia/Core/String.h"
#include <boost/format.hpp>

#ifndef NDEBUG
#include "Exception.h"
#endif

namespace sequoia {

/// @addtogroup core
/// @{

namespace core {

namespace internal {

template <class String>
struct BoostFormat {
  using type = boost::format;
};

template <>
struct BoostFormat<Utf16String> {
  using type = boost::wformat;
};

template <class StringType, class FormatType = typename BoostFormat<StringType>::type,
          typename CharT, typename... Args>
StringType basic_format(const CharT* fmt, Args&&... args) {
  FormatType f(fmt);

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

} // internal

/// @brief printf-like formatting of a string
///
/// @param fmt    Format string
/// @param args   Variadic sequence used as arguments
/// @{
template <typename... Args>
Utf8String format(const char* fmt, Args&&... args) {
  return internal::basic_format<Utf8String>(fmt, std::forward<Args>(args)...);
}

template <typename... Args>
Utf16String format(const wchar_t* fmt, Args&&... args) {
  return internal::basic_format<Utf16String>(fmt, std::forward<Args>(args)...);
}
/// @}

} // namespace core

/// @}

} // namespace sequoia

#endif
