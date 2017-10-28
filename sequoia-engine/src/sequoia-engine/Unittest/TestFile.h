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

#ifndef SEQUOIA_UNITTEST_TESTFILE_H
#define SEQUOIA_UNITTEST_TESTFILE_H

#include "sequoia-engine/Core/AlignedADT.h"
#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/File.h"
#include "sequoia-engine/Core/Platform.h"

namespace sequoia {

namespace unittest {

/// @brief Simple wrapper to an actual file on disk in `Environment::getRessourcePath()`
/// @ingroup unittest
class SEQUOIA_API TestFile : public File {
  platform::Path path_;
  aligned_vector<Byte> data_;

public:
  /// @brief Set the full path of the ressource file specified by `path` relative to the unittest
  /// ressource root (i.e `TestEnvironment::getRessourcePath()`)
  ///
  /// @param path   Path relative to the unittest ressource root
  /// @param type   Type of the file (if `FileType::Unknown` is passed, the type is deduced from the
  ///               extension of `path`
  TestFile(const platform::Path& path, FileType type = FileType::Unknown);

  /// @copydoc File::getData
  const Byte* getData() override;

  /// @copydoc File::getNumBytes
  std::size_t getNumBytes() override;

  /// @copydoc File::getPath
  std::string getPath() const noexcept override;

  /// @copydoc File::hash
  std::size_t hash() const noexcept override;

  /// @copydoc File::equals
  bool equals(const File* other) const noexcept override;

  /// @copydoc File::getFilename
  std::string getFilename() const noexcept override;

  /// @copydoc File::getExtension
  std::string getExtension() const noexcept override;

private:
  void load();
};

} // namespace unittest

} // namespace sequoia

#endif
