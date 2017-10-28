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

#ifndef SEQUOIA_ENGINE_CORE_EXCEPTION_H
#define SEQUOIA_ENGINE_CORE_EXCEPTION_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Core/UtfString.h"
#include <exception>
#include <iosfwd>
#include <memory>

namespace sequoia {

namespace core {

/// @brief Base class for all sequoia library exceptions
/// @ingroup core
class SEQUOIA_API Exception : public std::exception {
public:
  /// @brief Initialize the exception
  ///
  /// @param line       Line the exception was thrown
  /// @param path       Path to the file the exception was thrown
  /// @param fmt        Format string describing the general cause of the current error
  /// @param args       Arguments to substitute in `fmt`
  /// @{
  template <typename... Args>
  Exception(int line, const char* path, const char* fmt, Args&&... args)
      : message_(format(fmt, std::forward<Args>(args)...)), line_(line), path_(path) {}

  template <typename... Args>
  Exception(int line, const char* path, const wchar_t* fmt, Args&&... args)
      : message_(format(fmt, std::forward<Args>(args)...)), line_(line), path_(path) {}
  /// @}

  /// @brief Copy constructor
  Exception(const Exception&) = default;

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
  /// @}driver
protected:
  UtfString message_; ///< Explanatory string describing the general cause of the current error
  int line_;          ///< Line the exception was thrown
  const char* path_;  ///< Path to the file the exception was thrown
};

} // namespace core

/// @brief Declare a an exception `Type` which derives from `BaseType`
/// @ingroup core
#define SEQUOIA_DECLARE_EXCPETION_BASE(Type, BaseType)                                             \
  class Type : public BaseType {                                                                   \
  public:                                                                                          \
    template <typename... Args>                                                                    \
    Type(int line, const char* path, const char* fmt, Args&&... args)                              \
        : BaseType(line, path, fmt, std::forward<Args>(args)...) {}                                \
    template <typename... Args>                                                                    \
    Type(int line, const char* path, const wchar_t* fmt, Args&&... args)                           \
        : BaseType(line, path, fmt, std::forward<Args>(args)...) {}                                \
    virtual ~Type() noexcept {}                                                                    \
    Type(const Type&) = default;                                                                   \
  };

/// @brief Declare a an exception `Type` deriving from `core::sequoia::Exception`
/// @ingroup core
#define SEQUOIA_DECLARE_EXCPETION(Type)                                                            \
  SEQUOIA_DECLARE_EXCPETION_BASE(Type, sequoia::core::Exception)

#define SEQUOIA_THROW_IMPL(exception, line, file, ...) throw exception(line, file, __VA_ARGS__)

/// @brief Throw an `exception` and add current line and file information in debug mode
///
/// @param exception    Exception to throw
/// @param ...          Format string describing the general cause of the current error
/// @ingroup core
#ifndef NDEBUG
#define SEQUOIA_THROW(exception, ...) SEQUOIA_THROW_IMPL(exception, __LINE__, __FILE__, __VA_ARGS__)
#else
#define SEQUOIA_THROW(exception, ...) SEQUOIA_THROW_IMPL(exception, -1, nullptr, __VA_ARGS__)
#endif

} // namespace sequoia

#endif
