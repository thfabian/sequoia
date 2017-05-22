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

#ifndef SEQUOIA_RENDER_VERTEXARRAYOBJECT_H
#define SEQUOIA_RENDER_VERTEXARRAYOBJECT_H

#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Render/Export.h"
#include "sequoia/Render/RenderSystemObject.h"
#include "sequoia/Render/Vertex.h"

namespace sequoia {

namespace render {

/// @brief Handling of layout and buffering of an array of vertices
///
/// @see RenderSystem::createVertexArrayObject
/// @ingroup render
class SEQUOIA_RENDER_API VertexArrayObject : public RenderSystemObject, public NonCopyable { 
protected: 
  /// Pointer to the vertex data
  void* dataPtr_;
  
  /// Number of vertices
  std::size_t numVertices_;
  
  /// Layout of each vertex
  VertexLayout layout_;

public:
  virtual ~VertexArrayObject();
  VertexArrayObject(RenderSystemKind kind);

  /// @brief Attach `numVertices` at `dataPtr` with given `layout` 
  ///
  /// @param dataPtr      Starting adress of the vertex data
  /// @param numVertices  Number of vertices (i.e the total amount of bytes is 
  ///                     `numVertices * layout.SizeOf`)
  /// @param layout       Layout of each vertex  
  ///
  /// This will create the underlying device hardware buffers. However, the data is only copied to
  /// the GPU once `updateGPU` is called.
  void attachVertexData(void* dataPtr, std::size_t numVertices, const VertexLayout& layout);

  /// @brief Update the data on the device to match the CPU data
  virtual void updateDevice() = 0;

  /// @brief Get the number of allocated vertices
  std::size_t getNumVertices() const { return numVertices_; }
  
  /// @brief Check if vertex data has been attached
  bool isValid() const;

protected:
  /// @brief Allocate `numVertices` vertices on the GPU
  virtual void attachVertexDataDevice() = 0;
};

} // namespace render

} // namespace sequoia

#endif
