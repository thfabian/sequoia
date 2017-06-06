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

#include "sequoia/Core/Exception.h"
#include "sequoia/Core/UtfString.h"
#include "sequoia/Unittest/Environment.h"
#include "sequoia/Unittest/TestFile.h"
#include <fstream>
#include <sstream>

namespace sequoia {

namespace unittest {

TestFile::TestFile(const char* path) : path_(path), content_(nullptr) {}

StringRef TestFile::getContent() {
  if(!content_) {
    std::string fullPath = platform::toAnsiString(Environment::getSingleton().getRessourcePath() /
                                                  platform::asPath(path_));

    std::ifstream file(fullPath.c_str());

    if(!file.is_open())
      SEQUOIA_THROW(core::Exception, "cannot load source: '%s'", fullPath.c_str());

    std::stringstream ss;
    ss << file.rdbuf();
    content_ = std::make_unique<std::string>(ss.str());
  }
  return StringRef(*content_);
}

const std::string& TestFile::getPath() const noexcept { return path_; }

std::size_t TestFile::hash() const noexcept { return std::hash<std::string>()(getPath()); }

bool TestFile::equals(const core::File& other) const noexcept {
  return getPath() == other.getPath();
}

} // namespace unittest

} // namespace sequoia
