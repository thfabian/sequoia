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

#ifndef SEQUOIA_CORE_EXCEPTION_H
#define SEQUOIA_CORE_EXCEPTION_H

#include "sequoia/Core/Export.h"
#include "sequoia/Core/UtfString.h"
#include <exception>
#include <iosfwd>
#include <memory>

namespace sequoia {

namespace core {

/// @brief Base class for all sequoia library exceptions
/// @ingroup core
class SEQUOIA_CORE_API Exception : public std::exception {
public:
  /// @brief Initialize the exception
  ///
  /// @param message    Explanatory string describing the general cause of the current error
  /// @param line       Line the exception was thrown
  /// @param path       Path to the file the exception was thrown
  Exception(const std::string& message, int line = -1, const char* path = nullptr);
  Exception(const std::wstring& message, int line = -1, const char* path = nullptr);

  /// @brief Virtual destructor
  virtual ~Exception() noexcept;

  /// @brief Return a C-style character string describing the general cause of the current error
  ///
  /// If `NDEBUG` is not defined, the path and line will be prepended. This function returns a
  /// \b copy of the underlying C-string and thus the returned string needs to be deallocated.
  virtual const char* what() const noexcept override;

  /// @brief Return the line the exception was thrown
  virtual int line() const noexcept;

  /// @brief Return the path to the file the exception was thrown
  virtual const char* path() const noexcept;

  /// @brief Return explanatory string describing the general cause of the current error
  virtual const UtfString& message() const noexcept;

  /// @brief Convert to stream by streaming Exception::what() into `stream`.
  /// @{
  SEQUOIA_API_EXPORT friend std::ostream& operator<<(std::ostream& stream,
                                                     const Exception& exception);
  SEQUOIA_API_EXPORT friend std::wostream& operator<<(std::wostream& stream,
                                                      const Exception& exception);
  /// @}

protected:
  UtfString message_;   ///< Explanatory string describing the general cause of the current error
  const char* whatStr_; ///< UTF-8 string of `message`
  int line_;            ///< Line the exception was thrown
  const char* path_;    ///< Path to the file the exception was thrown
};

} // namespace core

/// @macro SEQUOIA_DECLARE_EXCPETION_BASE
/// @brief Declare a an exception `Type` deriving from `BaseType`
/// @ingroup core
#define SEQUOIA_DECLARE_EXCPETION_BASE(Type, BaseType)                                             \
  class Type : public BaseType {                                                                   \
  public:                                                                                          \
    Type(const std::string& message, int line = -1, const char* path = nullptr)                    \
        : BaseType(message, line, path) {}                                                         \
    Type(const std::wstring& message, int line = -1, const char* path = nullptr)                   \
        : BaseType(message, line, path) {}                                                         \
    virtual ~Type() noexcept {}                                                                    \
  };

/// @macro SEQUOIA_DECLARE_EXCPETION
/// @brief Declare a an exception `Type` deriving from `core::sequoia::Exception`
/// @ingroup core
#define SEQUOIA_DECLARE_EXCPETION(Type)                                                            \
  SEQUOIA_DECLARE_EXCPETION_BASE(Type, sequoia::core::Exception)

#define SEQUOIA_THROW_IMPL(exception, message, line, file) throw exception(message, line, file)

/// @macro SEQUOIA_THROW
/// @brief Throw an `exception` and add current line and file information in debug mode
///
/// @param exception    Exception to throw (exception has to derive from sequoia::core::Exception)
/// @param message      String describing the general cause of the current error
/// @ingroup core
#ifndef NDEBUG
#define SEQUOIA_THROW(exception, message) SEQUOIA_THROW_IMPL(exception, message, __LINE__, __FILE__)
#else
#define SEQUOIA_THROW(exception, message) SEQUOIA_THROW_IMPL(exception, message, -1, nullptr)
#endif

} // namespace sequoia

#endif
