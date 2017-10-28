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

#include "sequoia-engine/Render/GL/GLVertexAttribute.h"
#include "sequoia-engine/Core/StringRef.h"
#include "sequoia-engine/Render/Exception.h"

namespace sequoia {

namespace render {

namespace {

static const char* AttributeNames[GLVertexAttribute::NumAttributes] = {
    "in_Position", "in_Normal", "in_TexCoord", "in_Color", "in_Tangent", "in_Bitangent"};

} // anonymous namespace

const char* GLVertexAttribute::name(unsigned int attribute) { return AttributeNames[attribute]; }

GLVertexAttribute::Attribute GLVertexAttribute::attribute(const char* name) {
  StringRef nameStr(name);

  for(unsigned int attr = 0; attr < NumAttributes; ++attr)
    if(nameStr == AttributeNames[attr])
      return (GLVertexAttribute::Attribute)attr;

  SEQUOIA_THROW(RenderSystemException, "invalid vertex attribute '{}' ", name);
  return NumAttributes;
}

bool GLVertexAttribute::isValid(const char* name) {
  StringRef nameStr(name);
  for(unsigned int attr = 0; attr < NumAttributes; ++attr)
    if(nameStr == AttributeNames[attr])
      return true;
  return false;
}

void GLVertexAttribute::forEach(std::function<void(unsigned int, const char*)> functor) {
  for(unsigned int attr = 0; attr < NumAttributes; ++attr)
    functor(attr, AttributeNames[attr]);
}

} // namespace render

} // namespace sequoia
