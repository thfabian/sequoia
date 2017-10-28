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

#include "sequoia-engine/Core/File.h"
#include "sequoia-engine/Core/StringSwitch.h"
#include "sequoia-engine/Core/Unreachable.h"
#include <mutex>
#include <unordered_map>

namespace sequoia {

namespace core {

static std::unordered_map<std::string, FileType> ExtensionMap;
static std::once_flag ExtensionMapInitFlag;

FileType File::TypeFromExtension(StringRef path) noexcept {

  std::call_once(ExtensionMapInitFlag, []() {
    std::vector<StringRef> extensionsVec;
#define FILE_TYPE(Name, IsBinary, Extensions)                                                      \
  extensionsVec.clear();                                                                           \
  StringRef(Extensions).split(extensionsVec, '|');                                                 \
  for(const auto& ext : extensionsVec)                                                             \
    ExtensionMap.emplace(ext.str(), FileType::Name);
#include "sequoia-engine/Core/FileType.inc"
#undef FILE_TYPE
  });

  std::string ext(path.substr(path.find_last_of(".")).str());
  auto it = ExtensionMap.find(ext);
  return it != ExtensionMap.end() ? it->second : FileType::Unknown;
}

const char* File::TypeToString(FileType type) noexcept {
  switch(type) {
#define FILE_TYPE(Name, IsBinary, Extensions)                                                      \
  case FileType::Name:                                                                             \
    return #Name;
#include "sequoia-engine/Core/FileType.inc"
#undef FILE_TYPE
  case FileType::Unknown:
    return "Unknown";
  default:
    sequoia_unreachable("invalid file-type");
  }
}

bool File::IsBinary(FileType type) noexcept {
  switch(type) {
#define FILE_TYPE(Name, IsBinary, Extensions)                                                      \
  case FileType::Name:                                                                             \
    return IsBinary;
#include "sequoia-engine/Core/FileType.inc"
#undef FILE_TYPE
  case FileType::Unknown:
    return false;
  default:
    sequoia_unreachable("invalid file-type");
  }
}

} // namespace core

} // namespace sequoia
