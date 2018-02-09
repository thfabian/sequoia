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

#ifndef SEQUOIA_ENGINE_CORE_FILEBUFFER_H
#define SEQUOIA_ENGINE_CORE_FILEBUFFER_H

#include "sequoia-engine/Core/HostBuffer.h"
#include <string>

namespace sequoia {

namespace core {

/// @brief Content of a file loaded via `FileSystem`
/// @ingroup core
class SEQUOIA_API FileBuffer final : public HostBuffer {
public:
  /// @brief Different formats of files
  enum FileFormat {
    FF_Text,  ///< ASCII text file
    FF_Binary ///< Binary file
  };

  /// @brief Allocate the FileBuffer with `numBytes` capacity
  FileBuffer(FileFormat format, const std::string& path, std::size_t numBytes);

  /// @brief Get the name to the file
  std::string getFilename() const;

  /// @brief Get the extension of the file
  std::string getExtension() const;

  /// @brief Get the path of the file
  const std::string& getPath() const { return path_; }

  /// @brief set the path of the file
  void setPath(const std::string& path) { path_ = path; }

  /// @brief Get the path of the file
  FileFormat getFileFormat() const { return format_; }

  /// @brief Get a **copy** of the data as a string
  std::string getDataAsString() const { return std::string(getDataAs<char>(), getNumBytes()); }

protected:
  /// @copydoc Buffer::toStringImpl
  virtual std::pair<std::string, std::string> toStringImpl() const override;

private:
  /// Format of the file
  FileFormat format_;

  /// Path of the file
  std::string path_;
};

} // namespace core

} // namespace sequoia

#endif
