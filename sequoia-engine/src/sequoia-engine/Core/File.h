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

#ifndef SEQUOIA_CORE_FILE_H
#define SEQUOIA_CORE_FILE_H

#include "sequoia-engine/Core/Byte.h"
#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/StringRef.h"
#include <array>
#include <functional>
#include <memory>
#include <string>

namespace sequoia {

namespace core {

/// @brief Supported file types
/// @ingroup core
enum class FileType {
  Unknown,
#define FILE_TYPE(Name, IsBinary, Extensions) Name,
#include "sequoia-engine/Core/FileType.inc"
#undef FILE_TYPE
};

/// @brief File interface
///
/// File has specialization for `std::hash<File>` as well as `std::hash<std::shared_ptr<File>>`.
///
/// @ingroup core
class SEQUOIA_API File {
public:
  /// @brief Initialize the file
  /// @param type   Type of the file
  File(FileType type) : type_(type) {}

  /// @brief Virtual destructor
  virtual ~File() {}

  /// @brief Get the content of the file
  virtual const Byte* getData() = 0;

  /// @brief Get the number of bytes stored in the file
  virtual std::size_t getNumBytes() = 0;

  /// @brief Get a **copy** of the data as a string
  std::string getDataAsString() { return std::string((const char*)getData(), getNumBytes()); }

  /// @brief Get the path to the file (this may return a relative path)
  virtual std::string getPath() const noexcept = 0;

  /// @brief Get the name to the file
  virtual std::string getFilename() const noexcept = 0;

  /// @brief Get the extension to the file
  virtual std::string getExtension() const noexcept = 0;

  /// @brief Compute hash of the path of the file
  virtual std::size_t hash() const noexcept = 0;

  /// @brief Check if `this` is equal to `other`
  virtual bool equals(const File* other) const noexcept = 0;

  /// @brief Comparison operator
  /// @{
  bool operator==(const File& other) { return equals(&other); }
  bool operator!=(const File& other) { return !equals(&other); }
  /// @}

  /// @brief Get the type of the file
  FileType getType() const noexcept { return type_; }

  /// @brief Check if the file is a binary file
  bool isBinary() const noexcept { return File::IsBinary(type_); }

  /// @brief Get the FileType from the extension of `path`
  static FileType TypeFromExtension(StringRef path) noexcept;

  /// @brief Get the name of the FileType `type`
  static const char* TypeToString(FileType type) noexcept;

  /// @brief Check if the given FileType `type` is a binary format
  static bool IsBinary(FileType type) noexcept;

private:
  FileType type_;
};

/// @name Comparison operator for Files
/// @ingroup core
/// @{
inline bool operator==(const File& a, const File& b) noexcept { return a.equals(&b); }
inline bool operator==(const std::shared_ptr<File>& a, const std::shared_ptr<File>& b) noexcept {
  return (*a == *b);
}
inline bool operator!=(const File& a, const File& b) noexcept { return !a.equals(&b); }
inline bool operator!=(const std::shared_ptr<File>& a, const std::shared_ptr<File>& b) noexcept {
  return (*a != *b);
}
/// @}

} // namespace core

using File = core::File;
using FileType = core::FileType;

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
