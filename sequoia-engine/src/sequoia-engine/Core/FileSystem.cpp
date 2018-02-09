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

#include "sequoia-engine/Core/FileSystem.h"
#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Core/StringUtil.h"

namespace sequoia {

namespace core {

FileSystem::FileSystem(const std::string& baseDir) : baseDir_(baseDir) {}

std::string FileSystem::toString() const {
  auto stringPair = toStringImpl();
  return core::format("{}[\n  {}]", stringPair.first, core::indent(stringPair.second));
}

std::pair<std::string, std::string> FileSystem::toStringImpl() const {
  return std::make_pair("FileSystem", core::format("baseDir = \"{}\",\n", baseDir_));
}

} // namespace core

} // namespace sequoia
