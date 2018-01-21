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

#ifndef SEQUOIA_ENGINE_RENDER_D3D12_D3D12VERTEXDATA_H
#define SEQUOIA_ENGINE_RENDER_D3D12_D3D12VERTEXDATA_H

#include "sequoia-engine/Render/D3D12/D3D12IndexBuffer.h"
#include "sequoia-engine/Render/D3D12/D3D12VertexBuffer.h"
#include "sequoia-engine/Render/VertexData.h"

namespace sequoia {

namespace render {

/// @brief D3D12 Vertex Array
/// @ingroup d3d12
class SEQUOIA_API D3D12VertexData final : public VertexData {
public:
  /// @brief Allocate a vertex array object (VAO) with a vertex and index buffer
  D3D12VertexData(const VertexDataParameter& param);

  /// @brief Deallocate all memory
  ~D3D12VertexData();

  /// @copydoc VertexData::getVertexBuffer
  virtual VertexBuffer* getVertexBuffer() const override;

  /// @copydoc VertexData::getIndexBuffer
  virtual IndexBuffer* getIndexBuffer() const override;

  SEQUOIA_D3D12_OBJECT(VertexData)

protected:
  /// @brief Implementation of `toString` returns stringified members and title
  virtual std::pair<std::string, std::string> toStringImpl() const override;

private:
  /// Allocated VertexBuffer
  std::unique_ptr<D3D12VertexBuffer> vertexBuffer_;

  /// Allocated IndexBuffer (possibly D3D12)
  std::unique_ptr<D3D12IndexBuffer> indexBuffer_;

private:
  using Base = VertexData;
};

} // namespace render

} // namespace sequoia

#endif
