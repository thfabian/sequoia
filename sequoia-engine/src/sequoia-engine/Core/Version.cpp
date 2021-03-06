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

#include "sequoia-engine/Core/Version.h"
#include <iostream>

namespace sequoia {

namespace core {

Version::Version(int major, int minor, int patch) : major_(major), minor_(minor), patch_(patch) {}

Version::Version(int version) { *this = Version::fromSingle(version); }

int Version::toSingle(const Version& version) noexcept {
  return 100000 * version.major() + 100 * version.minor() + version.patch();
}

Version Version::fromSingle(int version) noexcept {
  return Version(version / 100000, version / 100 % 1000, version % 100);
}

bool Version::operator==(const Version& version) const noexcept {
  return (toSingle(*this) == version.toSingle(version));
}

bool Version::operator!=(const Version& version) const noexcept {
  return (toSingle(*this) != version.toSingle(version));
}

bool Version::operator<(const Version& version) const noexcept {
  return (toSingle(*this) < version.toSingle(version));
}

bool Version::operator<=(const Version& version) const noexcept {
  return (toSingle(*this) <= version.toSingle(version));
}

bool Version::operator>(const Version& version) const noexcept {
  return (toSingle(*this) > version.toSingle(version));
}

bool Version::operator>=(const Version& version) const noexcept {
  return (toSingle(*this) >= version.toSingle(version));
}

std::string Version::toString() const {
  return std::to_string(major_) + "." + std::to_string(minor_) + "." + std::to_string(patch_);
}

std::ostream& operator<<(std::ostream& stream, const Version& version) {
  return (stream << version.toString());
}

Version getSequoiaEngineVersion() noexcept {
  static_assert(SEQUOIA_ENGINE_VERSION_MAJOR < 10 && SEQUOIA_ENGINE_VERSION_MAJOR >= 0,
                "invalid major version, should be in [0, 10)");
  static_assert(SEQUOIA_ENGINE_VERSION_MINOR < 100 && SEQUOIA_ENGINE_VERSION_MINOR >= 0,
                "invalid minor version, should be in [0, 100)");
  static_assert(SEQUOIA_ENGINE_VERSION_PATCH < 10 && SEQUOIA_ENGINE_VERSION_PATCH >= 0,
                "invalid patch version, should be in [0, 10)");
  return Version(SEQUOIA_ENGINE_VERSION_MAJOR, SEQUOIA_ENGINE_VERSION_MINOR,
                 SEQUOIA_ENGINE_VERSION_PATCH);
}

const char* getSequoiaEngineFullVersionString() noexcept {
  return SEQUOIA_ENGINE_FULL_VERSION_STRING;
}

} // namespace core

} // namespace sequoia
