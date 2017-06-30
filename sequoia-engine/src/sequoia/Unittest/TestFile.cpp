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

#include "sequoia/Unittest/TestFile.h"
#include "sequoia/Core/Exception.h"
#include "sequoia/Core/Memory.h"
#include "sequoia/Core/StringRef.h"
#include "sequoia/Core/UtfString.h"
#include "sequoia/Unittest/Environment.h"
#include <fstream>
#include <sstream>

namespace sequoia {

namespace unittest {

TestFile::TestFile(const char* path, FileType type)
    : File(File::TypeFromExtension(path)), path_(path) {}

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
  std::string fullPath = platform::toAnsiString(Environment::getSingleton().getRessourcePath() /
                                                platform::asPath(path_));
  std::ios_base::openmode mode = std::ios_base::in;
  if(isBinary())
    mode |= std::ios_base::binary;

  std::ifstream file(fullPath.c_str(), mode);

  if(!file.is_open())
    SEQUOIA_THROW(core::Exception, "cannot load file: '%s'", path_.c_str());

  // Allocate memory
  file.seekg(0, std::ios_base::end);
  data_.resize(file.tellg());
  file.seekg(0, std::ios_base::beg);

  // Read ASCII file
  file.read(reinterpret_cast<char*>(data_.data()), data_.size());
}

const std::string& TestFile::getPath() const noexcept { return path_; }

std::size_t TestFile::hash() const noexcept { return std::hash<std::string>()(getPath()); }

bool TestFile::equals(const core::File& other) const noexcept {
  return getPath() == other.getPath();
}

StringRef TestFile::getFilename() const noexcept {
  StringRef str(path_);
  return str.substr(str.find_last_of("/\\") + 1);
}

StringRef TestFile::getExtension() const noexcept {
  StringRef str(path_);
  return str.substr(str.find_last_of("."));
}

} // namespace unittest

} // namespace sequoia
