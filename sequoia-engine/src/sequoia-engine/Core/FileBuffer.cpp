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

#include "sequoia-engine/Core/FileBuffer.h"
#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Core/Platform.h"

namespace sequoia {

namespace core {

FileBuffer::FileBuffer(const std::string& path, std::size_t numBytes) : path_(path) {
  allocate(numBytes, UH_Dynamic);
}

std::string FileBuffer::getFilename() const {
  return platform::toAnsiString(platform::Path(path_).filename());
}

std::string FileBuffer::getExtension() const {
  return platform::toAnsiString(platform::Path(path_).extension());
}

std::pair<std::string, std::string> FileBuffer::toStringImpl() const {
  return std::make_pair("FileBuffer", core::format("{}"
                                                   "path = \"{}\"\n",
                                                   Base::toStringImpl().second, path_));
}

} // namespace core

} // namespace sequoia
