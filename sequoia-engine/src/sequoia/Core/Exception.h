#ifndef SEQUOIA_CORE_EXCEPTION
#define SEQUOIA_CORE_EXCEPTION

#include "sequoia/Core/String.h"
#include <exception>
#include <iosfwd>
#include <memory>

namespace sequoia {

/// @addtogroup core
/// @{

namespace core {

//===--------------------------------------------------------------------------------------------===
//  Exception
//===--------------------------------------------------------------------------------------------===

/// @brief Base class for all library exceptions
class Exception : public std::exception {
public:
  /// @brief Initialize the exception
  ///
  /// @param whatArg    Explanatory string describing the general cause of the current error
  /// @param line       Line the exception was thrown
  /// @param path       Path to the file the exception was thrown
  Exception(const Utf8String& whatArg, int line = -1, const char* path = nullptr);
  Exception(const Utf16String& whatArg, int line = -1, const char* path = nullptr);
  
  /// @brief Virtual destructor
  virtual ~Exception() noexcept;

  /// @brief Return a C-style character string describing the general cause of the current error
  virtual const char* what() const noexcept;

  /// @brief Return the line the exception was thrown
  virtual int line() const noexcept;

  /// @brief Return the path to the file the exception was thrown
  virtual const char* path() const noexcept;

  /// @brief Convert to stream
  /// @{
  friend std::ostream& operator<<(std::ostream& stream, const Exception& exception);
  friend std::wostream& operator<<(std::wostream& stream, const Exception& exception);
  /// @}
  
protected:
  std::string whatArg_;
  int line_;
  const char* path_;
};

//===--------------------------------------------------------------------------------------------===
//  IOException
//===--------------------------------------------------------------------------------------------===

} // namespace core

#define SEQUOIA_THROW_IMPL(exception, message, line, file) throw exception(message, line, file)

/// @macro SEQUOIA_THROW
/// @brief Throw an `exception` and add current line and file information in debug mode
///
/// @param exception    Exception to throw (exception has to derive from sequoia::core::Exception)
/// @param message      String describing the general cause of the current error
#ifndef NDEBUG
#define SEQUOIA_THROW(exception, message) SEQUOIA_THROW_IMPL(exception, message, __LINE__, __FILE__)
#else
#define SEQUOIA_THROW(exception, message) SEQUOIA_THROW_IMPL(exception, message, -1, nullptr)
#endif

/// @}

using Exeption = core::Exception;

} // namespace sequoia

#endif
