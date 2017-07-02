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

#include "sequoia/Core/StringRef.h"
#include "sequoia/Render/Exception.h"
#include "sequoia/Render/GL/GLFragmentData.h"

namespace sequoia {

namespace render {

namespace {

static const char* DataNames[GLFragmentData::NumAttributes] = {"out_Color"};

static GLenum AttachmentNames[GLFragmentData::NumAttributes] = {GL_COLOR_ATTACHMENT0};

} // anonymous namespace

GLenum GLFragmentData::getAttachment(unsigned int data) { return AttachmentNames[data]; }

const char* GLFragmentData::name(unsigned int data) { return DataNames[data]; }

GLFragmentData::Data GLFragmentData::data(const char* name) {
  StringRef nameStr(name);

  for(unsigned int attr = 0; attr < NumAttributes; ++attr)
    if(nameStr == DataNames[attr])
      return (GLFragmentData::Data)attr;

  SEQUOIA_THROW(RenderSystemException, "invalid fragment data '%s' ", name);
  return NumAttributes;
}

bool GLFragmentData::isValid(const char* name) {
  StringRef nameStr(name);
  for(unsigned int attr = 0; attr < NumAttributes; ++attr)
    if(nameStr == DataNames[attr])
      return true;
  return false;
}

void GLFragmentData::forEach(std::function<void(unsigned int, const char*)> functor) {
  for(unsigned int attr = 0; attr < NumAttributes; ++attr)
    functor(attr, DataNames[attr]);
}

} // namespace render

} // namespace sequoia
