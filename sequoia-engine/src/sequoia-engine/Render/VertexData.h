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

#ifndef SEQUOIA_ENGINE_RENDER_VERTEXDATA_H
#define SEQUOIA_ENGINE_RENDER_VERTEXDATA_H

#include "sequoia-engine/Core/NonCopyable.h"
#include "sequoia-engine/Core/STLExtras.h"
#include "sequoia-engine/Math/AxisAlignedBox.h"
#include "sequoia-engine/Render/IndexBuffer.h"
#include "sequoia-engine/Render/RenderSystemObject.h"
#include "sequoia-engine/Render/Vertex.h"
#include "sequoia-engine/Render/VertexBuffer.h"

namespace sequoia {

namespace render {

/// @brief Manage storage of vertices and indices
/// @ingroup render
class SEQUOIA_API VertexData : public RenderSystemObject, public NonCopyable {
public:
  /// @brief Draw mode
  enum DrawModeKind {
    DM_Triangles = 0 ///< Treats each triplet of vertices as an independent triangle
  };

  VertexData(RenderSystemKind renderSystemKind, DrawModeKind drawMode);

  /// @brief Deallocate all memory
  virtual ~VertexData();

  /// @brief Get the draw-mode
  DrawModeKind getDrawMode() const noexcept { return drawMode_; }

  /// @brief Get the layout of the vertices
  const VertexLayout& getLayout() const noexcept { return getVertexBuffer()->getLayout(); }

  /// @brief Get the axis aligned bounding box
  const math::AxisAlignedBox& getAxisAlignedBox() const noexcept {
    SEQUOIA_ASSERT_MSG(bbox_, "bounding-box not set");
    return *bbox_;
  }

  /// @brief Set the axis aligned bounding box
  void setAxisAlignedBox(const math::AxisAlignedBox& bbox) {
    if(bbox_)
      *bbox_ = bbox;
    else
      bbox_ = std::make_unique<math::AxisAlignedBox>(bbox);
  }

  /// @brief Get number of allocated vertices
  std::size_t getNumVertices() const noexcept { return getVertexBuffer()->getNumVertices(); }

  /// @brief Check if IndexBuffer is allocated
  bool hasIndices() const { return getIndexBuffer() != nullptr; }

  /// @brief Get number of allocated indices
  std::size_t getNumIndices() const noexcept {
    return getIndexBuffer() ? getIndexBuffer()->getNumIndices() : 0;
  }

  /// @brief Get the VertexBuffer
  virtual VertexBuffer* getVertexBuffer() const = 0;

  /// @brief Get the IndexBuffer
  virtual IndexBuffer* getIndexBuffer() const = 0;

  /// @brief Dump the vertex data and indices to `stdout`
  void dump() const;

  /// @brief Convert to string
  std::string toString() const;

protected:
  /// @brief Implementation of `toString` returns stringified members and title
  virtual std::pair<std::string, std::string> toStringImpl() const;

private:
  /// Axis aligned bounding box of the mesh
  std::unique_ptr<math::AxisAlignedBox> bbox_;

  /// Mode of drawing the vertices
  DrawModeKind drawMode_;
};

/// @brief Parameter used to initialize the VertexData
/// @ingroup render
struct VertexDataParameter {

  /// Mode used to draw the vertices
  VertexData::DrawModeKind DrawMode;

  /// Layout of the vertices
  VertexLayout Layout;

  /// Number of vertices to allocate
  std::size_t NumVertices;

  /// Number of indices to allocate
  std::size_t NumIndices;

  /// Usage hint of the vertex-buffer
  core::Buffer::UsageHint VertexBufferUsageHint;

  /// Use a shadow buffer for the vertices?
  bool UseVertexShadowBuffer = true;

  /// Use a shadow buffer for the indices?
  bool UseIndexShadowBuffer = false;

  /// Type of indices
  IndexBuffer::IndexType IndexType = IndexBuffer::IT_UInt32;

  VertexDataParameter(VertexData::DrawModeKind drawMode, VertexLayout layout,
                      std::size_t numVertices, std::size_t numIndices,
                      core::Buffer::UsageHint vertexBufferUsageHint)
      : DrawMode(drawMode), Layout(std::move(layout)), NumVertices(numVertices),
        NumIndices(numIndices), VertexBufferUsageHint(vertexBufferUsageHint) {}

  VertexDataParameter(const VertexDataParameter&) = default;
  VertexDataParameter(VertexDataParameter&&) = default;
};

} // namespace render

} // namespace sequoia

#endif
