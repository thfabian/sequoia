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

#include "sequoia/Render/Null/NullVertexArrayObject.h"

namespace sequoia {

namespace render {

NullVertexArrayObject::~NullVertexArrayObject() {}

NullVertexArrayObject::NullVertexArrayObject() : VertexArrayObject(RK_Null) {}

void NullVertexArrayObject::writeVertexData(std::size_t offset, std::size_t length) {}

void NullVertexArrayObject::writeIndexData(std::size_t offset, std::size_t length) {}

std::string NullVertexArrayObject::toString() const { return "NullVertexArrayObject[]"; }

void NullVertexArrayObject::attachVertexDataDevice() {}

void NullVertexArrayObject::freeVertexDataDevice() {}

} // namespace render

} // namespace sequoia
