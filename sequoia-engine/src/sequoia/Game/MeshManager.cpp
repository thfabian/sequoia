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

#include "sequoia/Core/Memory.h"
#include "sequoia/Game/MeshManager.h"

namespace sequoia {

namespace game {

static void allocate(Mesh::Data* data, std::size_t numVertices,
                     const render::VertexLayout* layout) {
  data->DataPtr = memory::aligned_alloc(numVertices * layout->SizeOf);
  data->NumVertices = numVertices;
}

static void free(Mesh::Data* data) noexcept {
  memory::aligned_free(data->DataPtr);
  data->NumVertices = 0;
  data->Layout = nullptr;
  data->AAB.clear();
}

std::shared_ptr<Mesh> MeshManager::load(const std::string& name, const platform::String& path,
                                        render::VertexArrayObject::BufferUsageKind usage) {
  return nullptr;
}

} // namespace game

} // namespace sequoia
