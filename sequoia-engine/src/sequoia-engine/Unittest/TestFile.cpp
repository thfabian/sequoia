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

#include "sequoia-engine/Core/Exception.h"
#include "sequoia-engine/Core/Memory.h"
#include "sequoia-engine/Core/StringRef.h"
#include "sequoia-engine/Core/UtfString.h"
#include "sequoia-engine/Unittest/TestEnvironment.h"
#include "sequoia-engine/Unittest/TestFile.h"
#include <fstream>
#include <sstream>

namespace sequoia {

namespace unittest {

TestFile::TestFile(const platform::Path& path, FileType type)
    : File(type == FileType::Unknown
               ? File::TypeFromExtension(platform::toAnsiString(path.extension()))
               : type),
      path_(path) {}

const Byte* TestFile::getData() {
  if(data_.empty())
    load();
  return data_.data();
}

std::size_t TestFile::getNumBytes() {
  if(data_.empty())
    load();
  return data_.size();
}

void TestFile::load() {
  std::ios_base::openmode mode = std::ios_base::in;
  if(isBinary())
    mode |= std::ios_base::binary;

  std::ifstream file(platform::toAnsiString(path_).c_str(), mode);
  if(!file.is_open())
    SEQUOIA_THROW(core::Exception, "cannot load file: '{}'", path_.c_str());

  // Allocate memory
  file.seekg(0, std::ios_base::end);
  data_.resize(file.tellg());
  file.seekg(0, std::ios_base::beg);

  // Read ASCII file
  file.read(reinterpret_cast<char*>(data_.data()), data_.size());
}

std::string TestFile::getPath() const noexcept { return platform::toAnsiString(path_); }

std::size_t TestFile::hash() const noexcept { return std::hash<std::string>()(getPath()); }

bool TestFile::equals(const core::File* other) const noexcept {
  return getPath() == other->getPath();
}

std::string TestFile::getFilename() const noexcept {
  return platform::toAnsiString(path_.filename());
}

std::string TestFile::getExtension() const noexcept {
  return platform::toAnsiString(path_.extension());
}

} // namespace unittest

} // namespace sequoia
