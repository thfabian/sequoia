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

#ifndef SEQUOIA_RENDER_VERTEXARRAY_H
#define SEQUOIA_RENDER_VERTEXARRAY_H

#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Render/Export.h"
#include "sequoia/Render/RenderSystemObject.h"
#include "sequoia/Render/Vertex.h"
#include <cstdint>

namespace sequoia {

namespace render {

/// @brief Array of vertices which handles memory management on CPU and GPU respectively
///
/// Optionally the indices on how to draw the vertices are stored.
///
/// @see RenderSystem::generateVertexArray
/// @ingroup render
class SEQUOIA_RENDER_API VertexArray : public RenderSystemObject, public NonCopyable {
  /// Array of vertices
  Vertex* vertices_;

  /// Number of vertices
  std::size_t numVertices_;

public:
  virtual ~VertexArray();
  VertexArray(RenderSystemKind kind);

  /// @brief Allocate `numVertices` vertices
  ///
  /// This will also create the underlying GPU hardware buffers. However, the data is only copied to
  /// the GPU once `updateGPU` is called.
  void allocate(std::size_t numVertices);

  /// @brief Update the data on the GPU to match the CPU data
  virtual void updateGPU() = 0;

  /// @brief Get the vertex pointer
  Vertex* getVertexPointer() { return vertices_; }
  const Vertex* getVertexPointer() const { return vertices_; }

  /// @brief Get the number of allocated vertices
  std::size_t getNumVertices() const { return numVertices_; }

protected:
  /// @brief Allocate `numVertices` vertices on the GPU
  virtual void allocateGPU(std::size_t numVertices) = 0;
};

} // namespace render

} // namespace sequoia

#endif
