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

#ifndef SEQUOIA_UNITTEST_TESTFILE_H
#define SEQUOIA_UNITTEST_TESTFILE_H

#include "sequoia/Core/File.h"
#include "sequoia/Unittest/Export.h"

namespace sequoia {

namespace unittest {

/// @brief Simple wrapper to an actual file on disk in `Environment::getRessourcePath()`
/// @ingroup unittest
class SEQUOIA_UNITTEST_API TestFile : public File {
  std::string path_;
  std::unique_ptr<std::string> content_;

public:
  /// @brief Set the full path of the ressource file specified by `path` relative to the unittest
  /// ressource root (i.e `Environment::getRessourcePath()`)
  ///
  /// @param path   Path relative to the unittest ressource root
  TestFile(const char* path);

  /// @copydoc File::getContent
  virtual StringRef getContent() override;

  /// @copydoc File::getPath
  virtual const std::string& getPath() const noexcept override;

  /// @copydoc File::hash
  std::size_t hash() const noexcept override;

  /// @copydoc File::equals
  bool equals(const File& other) const noexcept override;
};

} // namespace unittest

} // namespace sequoia

#endif