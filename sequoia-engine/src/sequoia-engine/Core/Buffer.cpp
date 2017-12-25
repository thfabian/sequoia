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

#include "sequoia-engine/Core/Buffer.h"
#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Core/StringUtil.h"
#include <cstring>

namespace sequoia {

namespace core {

static std::string usageHintToString(Buffer::UsageHint usage) {
  if(usage == Buffer::UH_Invalid)
    return "invalid";

  std::vector<const char*> usages;

  if(usage & Buffer::UH_Static)
    usages.push_back("Static");
  if(usage & Buffer::UH_Dynamic)
    usages.push_back("Dynamic");
  if(usage & Buffer::UH_WriteOnly)
    usages.push_back("WriteOnly");
  if(usage & Buffer::UH_Discardable)
    usages.push_back("Discardable");

  return core::RangeToString(", ", "{", "}")(usages);
}

Buffer::Buffer(BufferKind kind)
    : data_(nullptr), numBytes_(0), usageHint_(UH_Invalid), shadowBufferIsDirty_(false),
      shadowBuffer_(nullptr), kind_(kind) {}

Buffer::~Buffer() {}

void Buffer::write(const void* src, std::size_t offset, std::size_t length, bool discardBuffer) {
  if(hasShadowBuffer())
    shadowBuffer_->writeImpl(src, offset, length, discardBuffer);
  writeImpl(src, offset, length, discardBuffer);
}

void Buffer::read(std::size_t offset, std::size_t length, void* dest) {
  if(hasShadowBuffer())
    shadowBuffer_->readImpl(offset, length, dest);
  else
    readImpl(offset, length, dest);
}

std::string Buffer::toString() const {
  auto stringPair = toStringImpl();
  return core::format("{}[\n  {}]", stringPair.first, core::indent(stringPair.second));
}

void Buffer::updateFromShadow() {
  SEQUOIA_ASSERT_MSG(hasShadowBuffer(), "cannot update from non-existing shadow buffer");

  if(shadowBufferIsDirty_) {

    // Lock the shadow buffer and copy it's content to our buffer
    shadowBuffer_->lock(LO_ReadOnly);

    void* src = shadowBuffer_->get();
    writeImpl(src, 0, numBytes_, true);

    shadowBuffer_->unlock();

    shadowBufferIsDirty_ = false;
  }
}

std::pair<std::string, std::string> Buffer::toStringImpl() const {
  return std::make_pair("Buffer", core::format("numBytes = {},\n"
                                               "usage = {},\n"
                                               "shadowBuffer = {},\n",
                                               numBytes_, usageHintToString(usageHint_),
                                               shadowBuffer_ ? shadowBuffer_->toString() : "null"));
}

} // namespace core

} // namespace sequoia
