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
#include "sequoia-engine/Core/Unreachable.h"

namespace sequoia {

namespace core {

namespace {

static const char* formatToString(FileBuffer::FileFormat format) {
  switch(format) {
  case FileBuffer::FF_Text:
    return "Text";
  case FileBuffer::FF_Binary:
    return "Binary";
  default:
    sequoia_unreachable("invalid format");
  }
}

} // anonymous namespace

FileBuffer::FileBuffer(FileFormat format, const std::string& path, std::size_t numBytes)
    : format_(format), path_(path) {
  allocate(numBytes, UH_Dynamic);
}

std::string FileBuffer::getFilename() const {
  return platform::toAnsiString(platform::Path(path_).filename());
}

std::string FileBuffer::getExtension() const {
  return platform::toAnsiString(platform::Path(path_).extension());
}

std::pair<std::string, std::string> FileBuffer::toStringImpl() const {
  return std::make_pair("FileBuffer",
                        core::format("{}"
                                     "format = {},\n"
                                     "path = \"{}\"\n",
                                     Base::toStringImpl().second, formatToString(format_), path_));
}

} // namespace core

} // namespace sequoia
