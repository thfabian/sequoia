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

#ifndef SEQUOIA_ENGINE_CORE_FILESYSTEM_H
#define SEQUOIA_ENGINE_CORE_FILESYSTEM_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/FileBuffer.h"
#include "sequoia-engine/Core/StringRef.h"
#include <cstdint>
#include <unordered_map>

namespace sequoia {

namespace core {

/// @brief Abstract file system interface
/// @ingroup core
class SEQUOIA_API FileSystem {
public:
  FileSystem(const std::string& baseDir);

  /// @brief Virtual destructor
  virtual ~FileSystem() {}

  /// @brief Read the file at `path` and return it's content as a `FileBuffer`
  ///
  /// @param path     Path to the file (relative to `getBaseDir()`)
  /// @param format   Format of opening the file
  ///
  /// @throws Exception   Read operation failed
  /// @returns content of the file at `path`
  virtual std::shared_ptr<FileBuffer> read(StringRef path, FileBuffer::FileFormat format) = 0;

  /// @brief Write the content of `buffer` to the file `path`
  ///
  /// @param path     Path to the file (relative to `getBaseDir()`)
  /// @param buffer   Content of the file
  ///
  /// @throws Exception   Write operation failed
  virtual void write(StringRef path, const std::shared_ptr<FileBuffer>& buffer) = 0;

  /// @brief Check if the file at `path` exists
  virtual bool exists(StringRef path) = 0;

  /// @brief Create a virtual file `path` with content `buffer`
  ///
  /// @param path     Path to the file (we be rooted relative to `getBaseDir()`)
  /// @param format   Format of the file
  /// @param buffer   Content of the virtual file
  ///
  /// @throws Exception   Failed to add the virtual file `path`
  virtual void addFile(StringRef path, const std::shared_ptr<FileBuffer>& buffer) = 0;

  /// @brief Get the base directory
  const std::string& getBaseDir() const noexcept { return baseDir_; }

  /// @brief Convert to string
  std::string toString() const;

protected:
  /// @brief Implementation of `toString` returns stringified members and title
  virtual std::pair<std::string, std::string> toStringImpl() const;

private:
  std::string baseDir_;
};

} // namespace core

using FileSystem = core::FileSystem;

} // namespace sequoia

#endif
