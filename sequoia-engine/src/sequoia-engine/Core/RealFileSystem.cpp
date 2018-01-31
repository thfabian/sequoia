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

#include "sequoia-engine/Core/RealFileSystem.h"
#include "sequoia-engine/Core/Exception.h"
#include "sequoia-engine/Core/FileBuffer.h"
#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Core/StringUtil.h"
#include "sequoia-engine/Core/Unreachable.h"

namespace sequoia {

namespace core {

static const char* openModeToString(RealFileSystem::OpenModeKind mode) {
  switch(mode) {
  case RealFileSystem::OK_Invalid:
    return "Invalid";
  case RealFileSystem::OK_Read:
    return "Read";
  case RealFileSystem::OK_Write:
    return "Write";
  case RealFileSystem::OK_ReadAndWrite:
    return "ReadAndWrite";
  default:
    sequoia_unreachable("invalid OpenModeKind");
  }
}

//                     newMode
//          +-----+------+------+------+
//          |     |  R   |  W   |  RW  |
//          +-----+------+------+------+
//          |  R  |  R   |  RW  |  RW  |
// curMode  +-----+------+------+------+
//          |  W  |  RW  |  W   |  RW  |
//          +-----+------+------+------+
//          |  RW |  RW  |  RW  |  RW  |
//          +-----+------+------+------+
//
static RealFileSystem::OpenModeKind getNewMode(RealFileSystem::OpenModeKind curMode,
                                               RealFileSystem::OpenModeKind newMode) {
  if(curMode == RealFileSystem::OK_Invalid)
    return newMode;

  // OK_ReadAndWrite
  if(curMode == RealFileSystem::OK_ReadAndWrite)
    return RealFileSystem::OK_ReadAndWrite;

  // OK_Read || OK_Write
  return static_cast<RealFileSystem::OpenModeKind>(static_cast<int>(curMode) |
                                                   static_cast<int>(newMode));
}

std::string RealFileSystem::FileInfo::toString() const {
  return core::format("FileInfo[\n"
                      "  Buffer = {},\n"
                      "  IsRealFile = {},\n"
                      "  OpenMode = {}\n"
                      "]",
                      Buffer ? core::indent(Buffer->toString()) : "null",
                      IsRealFile ? "true" : "false", openModeToString(OpenMode));
}

RealFileSystem::RealFileSystem(const std::string& baseDir)
    : FileSystem(baseDir), baseDirPath_(platform::asPath(baseDir)) {}

RealFileSystem::~RealFileSystem() {}

std::shared_ptr<FileBuffer> RealFileSystem::read(StringRef path, FileBuffer::FileFormat format) {
  auto it = this->open(path.str(), OK_Read, format);
  if(it != cachedFileInfos_.end() && it->second->Buffer)
    return it->second->Buffer;
  return it != cachedFileInfos_.end() ? readImpl(format, it) : nullptr;
}

void RealFileSystem::write(StringRef path, const std::shared_ptr<FileBuffer>& buffer) {
  writeImpl(open(path.str(), OK_Write, buffer->getFileFormat(), buffer));
}

bool RealFileSystem::exists(StringRef path) {
  return cachedFileInfos_.count(path.str()) ||
         platform::filesystem::exists(baseDirPath_ / platform::asPath(path));
}

void RealFileSystem::addFile(StringRef path, const std::shared_ptr<FileBuffer>& buffer) {
  SEQUOIA_ASSERT(0);
}

std::pair<std::string, std::string> RealFileSystem::toStringImpl() const {
  return std::make_pair(
      "RealFileSystem",
      core::format("{}"
                   "cachedFileInfos = {}\n",
                   Base::toStringImpl().second,
                   cachedFileInfos_.empty()
                       ? "null"
                       : core::toStringRange(cachedFileInfos_, [](const auto& var) {
                           return core::indent(core::format("{{\n"
                                                            "  path = \"{}\",\n"
                                                            "  info = {}\n"
                                                            "}}",
                                                            var.first,
                                                            core::indent(var.second->toString())));
                         })));
}

RealFileSystem::Iterator RealFileSystem::open(const std::string& path, OpenModeKind openMode,
                                              FileBuffer::FileFormat format,
                                              const std::shared_ptr<FileBuffer>& buffer) {

  platform::Path fullPath;
  auto makeFullPath = [this, &fullPath, &path]() -> const platform::Path& {
    if(fullPath.empty())
      fullPath = baseDirPath_ / platform::asPath(path);
    return fullPath;
  };
  FileInfo* info = nullptr;

  // Did we already open the file?
  auto it = cachedFileInfos_.find(path);
  if(it != cachedFileInfos_.end()) {
    // Yes!
    info = it->second.get();
    info->Mutex.lock();
  } else {
    // No, check the file exists and allocate the info
    if(!platform::filesystem::exists(makeFullPath()))
      SEQUOIA_THROW(Exception, "no such file: \"{}\"", path);

    auto fieldInfo = std::make_unique<FileInfo>();
    fieldInfo->Mutex.lock();
    it = cachedFileInfos_.emplace(path, std::move(fieldInfo)).first;
    info = it->second.get();
  }

  // Make sure the file is open in the correct mode
  OpenModeKind compatibleMode = getNewMode(info->OpenMode, openMode);
  if(!info->FileStream || compatibleMode != info->OpenMode) {
    if(info->FileStream) {
      info->FileStream.reset();
      info->Buffer.reset();
    }

    std::ios_base::openmode mode = std::ios_base::out;
    switch(compatibleMode) {
    case RealFileSystem::OK_Read:
      mode = std::ios_base::in;
      break;
    case RealFileSystem::OK_Write:
      mode = std::ios_base::out | std::ios_base::trunc;
      break;
    case RealFileSystem::OK_ReadAndWrite:
      mode = std::ios_base::in | std::ios_base::out;
      break;
    default:
      sequoia_unreachable("invalid OpenModeKind");
    }

    if(format == FileBuffer::FF_Binary)
      mode |= std::ios_base::binary;

    info->FileStream = std::make_unique<std::fstream>(makeFullPath().c_str(), mode);
    info->OpenMode = compatibleMode;
    if(!info->FileStream->is_open())
      SEQUOIA_THROW(Exception, "cannot open file: \"{}\"", path);
  }

  if(buffer)
    info->Buffer = buffer;

  info->Mutex.unlock();
  return it;
}

std::shared_ptr<FileBuffer> RealFileSystem::readImpl(FileBuffer::FileFormat format, Iterator it) {
  FileInfo* info = it->second.get();
  SEQUOIA_ASSERT(info->FileStream);
  SEQUOIA_LOCK_GUARD(info->Mutex);

  info->Buffer.reset();

  info->FileStream->seekg(0, std::ios_base::end);
  std::size_t numBytes = info->FileStream->tellg();
  info->FileStream->seekg(0, std::ios_base::beg);

  info->Buffer = std::make_unique<FileBuffer>(format, it->first, numBytes);
  info->FileStream->read(info->Buffer->getDataAs<char>(), info->Buffer->getNumBytes());
  if(info->FileStream->fail())
    SEQUOIA_THROW(Exception, "failed to read: \"{}\"", info->Buffer->getPath());

  return info->Buffer;
}

bool RealFileSystem::writeImpl(Iterator it) {
  FileInfo* info = it->second.get();
  SEQUOIA_ASSERT(info->Buffer);
  SEQUOIA_LOCK_GUARD(info->Mutex);

  info->FileStream->write(info->Buffer->getDataAs<char>(), info->Buffer->getNumBytes());
  if(info->FileStream->fail())
    SEQUOIA_THROW(Exception, "failed to write: \"{}\"", info->Buffer->getPath());

  return true;
}

} // namespace core

} // namespace sequoia
