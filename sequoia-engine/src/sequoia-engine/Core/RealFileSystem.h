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

#ifndef SEQUOIA_ENGINE_CORE_REALFILESYSTEM_H
#define SEQUOIA_ENGINE_CORE_REALFILESYSTEM_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/FileSystem.h"
#include "sequoia-engine/Core/Mutex.h"
#include "sequoia-engine/Core/Platform.h"
#include <fstream>

namespace sequoia {

namespace core {

/// @brief A file system mirroring the real operating system's file system
/// @ingroup core
class SEQUOIA_API RealFileSystem final : public FileSystem {
public:
  enum OpenModeKind {
    OK_Invalid = 0,
    OK_Read = 1 << 1,
    OK_Write = 1 << 2,
    OK_ReadAndWrite = OK_Read | OK_Write
  };

  RealFileSystem(const std::string& baseDir);
  virtual ~RealFileSystem();

  /// @copydoc FileSystem::read
  virtual std::shared_ptr<FileBuffer> read(StringRef path, FileBuffer::FileFormat format) override;

  /// @copydoc FileSystem::write
  virtual void write(StringRef path, const std::shared_ptr<FileBuffer>& buffer) override;

  /// @copydoc FileSystem::exists
  virtual bool exists(StringRef path) override;

  /// @copydoc FileSystem::addFile
  virtual void addFile(StringRef path, const std::shared_ptr<FileBuffer>& buffer) override;

protected:
  /// @copydoc FileSystem::toStringImpl
  virtual std::pair<std::string, std::string> toStringImpl() const override;

private:
  using Base = FileSystem;

  struct FileInfo {
    std::unique_ptr<std::fstream> FileStream = nullptr; ///< Stream to the opened file
    std::shared_ptr<FileBuffer> Buffer = nullptr;       ///< Content of the file
    bool IsRealFile = true;                             ///< Is it a real or added file?
    OpenModeKind OpenMode = OK_Invalid;                 ///< Mode of the opened file
    SpinMutex Mutex;                                    ///< Access mutex to modify the FileInfo

    /// @brief Convert to string
    std::string toString() const;
  };

  using FielInfoMap = std::unordered_map<std::string, std::unique_ptr<FileInfo>>;
  using Iterator = typename FielInfoMap::iterator;

  /// @brief Open the file `path`
  Iterator open(const std::string& path, OpenModeKind openMode, FileBuffer::FileFormat format,
                const std::shared_ptr<FileBuffer>& buffer = nullptr);

  /// @brief Read the `buffer` from the file
  std::shared_ptr<FileBuffer> readImpl(FileBuffer::FileFormat format, Iterator it);

  /// @brief Write the `buffer` to the file
  bool writeImpl(Iterator it);

private:
  /// Path representation
  platform::Path baseDirPath_;

  /// Cached FileInfo
  FielInfoMap cachedFileInfos_;
};

} // namespace core

using RealFileSystem = core::RealFileSystem;

} // namespace sequoia

#endif
