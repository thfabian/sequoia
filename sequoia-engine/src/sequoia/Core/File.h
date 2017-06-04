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

#ifndef SEQUOIA_CORE_FILE_H
#define SEQUOIA_CORE_FILE_H

#include "sequoia/Core/Export.h"
#include "sequoia/Core/StringRef.h"
#include <functional>
#include <memory>
#include <string>

namespace sequoia {

namespace core {

/// @brief File interface
/// @ingroup core
class SEQUOIA_CORE_API File {
public:
  /// @brief Get the content of the file
  /// @returns content or empty string if empty
  virtual StringRef getContent() = 0;

  /// @brief Check if file exists
  virtual bool exists() const noexcept = 0;

  /// @brief Get the path to the file (this may return a relative path)
  virtual const std::string& getPath() const noexcept = 0;

  /// @brief Compute hash of the path of the file
  virtual std::size_t hash() const noexcept = 0;

  /// @brief Check if `this` is equal to `other`
  virtual bool equals(const File& other) const noexcept = 0;

  /// @brief Comparison operator
  /// @{
  bool operator==(const File& other) { return equals(other); }
  bool operator!=(const File& other) { return !equals(other); }
  /// @}
};

/// @name Comparison operator for Files
/// @ingroup core
/// @{
inline bool operator==(const File& a, const File& b) noexcept { return a.equals(b); }
inline bool operator==(const std::shared_ptr<File>& a, const std::shared_ptr<File>& b) noexcept {
  return (*a == *b);
}
inline bool operator!=(const File& a, const File& b) noexcept { return !a.equals(b); }
inline bool operator!=(const std::shared_ptr<File>& a, const std::shared_ptr<File>& b) noexcept {
  return (*a != *b);
}
/// @}

} // namespace core

using File = core::File;

} // namespace sequoia

namespace std {

template <>
struct hash<sequoia::core::File> {
  std::size_t operator()(const sequoia::core::File& file) const { return file.hash(); }
};

template <>
struct hash<shared_ptr<sequoia::core::File>> {
  std::size_t operator()(const shared_ptr<sequoia::core::File>& file) const { return file->hash(); }
};

} // namespace std

#endif
