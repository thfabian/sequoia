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

#include "sequoia/Core/Format.h"
#include "sequoia/Core/StringUtil.h"
#include "sequoia/Render/Buffer.h"
#include <cstring>

namespace sequoia {

namespace render {

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

std::string Buffer::toString() const {
  auto stringPair = toStringImpl();
  return core::format("%s[\n  %s]", stringPair.first, core::indent(stringPair.second));
}

void Buffer::updateFromShadow() {
  SEQUOIA_ASSERT_MSG(hasShadowBuffer(), "cannot update from non-existing shadow buffer");

  if(shadowBufferIsDirty_) {

    // Lock the shadow buffer
    shadowBuffer_->lock(LK_ReadOnly);
    void* src = shadowBuffer_->get();

    // Lock our buffer
    lockImpl(LK_Discard);
    void* dest = get();

    // Copy the data
    std::memcpy(dest, src, numBytes_);

    // Unlock ourself and the shadow buffer
    unlockImpl();
    shadowBuffer_->unlock();

    shadowBufferIsDirty_ = false;
  }
}

std::pair<std::string, std::string> Buffer::toStringImpl() const {
  return std::make_pair("Buffer", core::format("numBytes = %s,\n"
                                               "usage = %s,\n"
                                               "shadowBuffer = %s,\n",
                                               numBytes_, usageHintToString(usageHint_),
                                               shadowBuffer_ ? shadowBuffer_->toString() : "null"));
}

} // namespace render

} // namespace sequoia