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

#ifndef SEQUOIA_CORE_VERSION_H
#define SEQUOIA_CORE_VERSION_H

#include "sequoia/Core/Config.h"
#include "sequoia/Core/Export.h"
#include <iosfwd>
#include <string>

namespace sequoia {

namespace core {

/// @brief Utility to compare semantic versions
///
/// @ingroup core
class SEQUOIA_API Version {
  int major_, minor_, patch_;

public:
  /// @brief Constrcut version
  Version(int major, int minor, int patch);
  Version(int version);
  Version(Version&&) = default;
  Version& operator=(const Version&) = default;
  Version& operator=(Version&&) = default;

  /// @brief Convert version to a single integer
  ///
  /// @code
  ///   auto version = Version::currentVersion();
  ///   version.toSingle(version) % 100;          // is the current patch level
  ///   version.toSingle(version) / 100 % 1000;   // is the current minor version
  ///   version.toSingle(version) / 100000;       // is the current major version
  /// @endcode
  static int toSingle(const Version& version) noexcept;

  /// @brief Convert integer to version tripple
  ///
  /// This is the reverse of Version::toSingle().
  static Version fromSingle(int version) noexcept;

  /// @brief Major revision
  int major() const noexcept { return major_; }

  /// @brief Minor revision
  int minor() const noexcept { return minor_; }

  /// @brief Patch revision
  int patch() const noexcept { return patch_; }

  /// @name Comparison operator
  /// @{
  bool operator==(const Version& version) const noexcept;
  bool operator!=(const Version& version) const noexcept;
  bool operator<(const Version& version) const noexcept;
  bool operator<=(const Version& version) const noexcept;
  bool operator>(const Version& version) const noexcept;
  bool operator>=(const Version& version) const noexcept;
  /// @}

  /// @brief Convert to string
  std::string toString() const;

  /// @brief Convert to stream
  SEQUOIA_API friend std::ostream& operator<<(std::ostream& stream, const Version& version);
};

/// @brief Get current version of the Sequoia Engine
extern SEQUOIA_API Version getSequoiaEngineVersion() noexcept;

/// @brief Get the full version string of the Sequoia Engine
extern SEQUOIA_API const char* getSequoiaEngineFullVersionString() noexcept;

} // namespace core

} // namespace sequoia

#endif
