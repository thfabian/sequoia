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

#ifndef SEQUOIA_RENDER_NULL_NULLVERTEXDATA_H
#define SEQUOIA_RENDER_NULL_NULLVERTEXDATA_H

#include "sequoia-engine/Render/Null/NullIndexBuffer.h"
#include "sequoia-engine/Render/Null/NullVertexBuffer.h"
#include "sequoia-engine/Render/VertexData.h"

namespace sequoia {

namespace render {

/// @brief Null Vertex Array
/// @ingroup null
class SEQUOIA_API NullVertexData final : public VertexData {
public:
  /// @brief Allocate a vertex array object (VAO) with a vertex and index buffer
  NullVertexData(const VertexDataParameter& param);

  /// @brief Deallocate all memory
  ~NullVertexData();

  /// @copydoc VertexData::getVertexBuffer
  virtual VertexBuffer* getVertexBuffer() const override;

  /// @copydoc VertexData::getIndexBuffer
  virtual IndexBuffer* getIndexBuffer() const override;

  /// @copydoc VertexData::nextTimestep
  virtual void nextTimestep() override;

  SEQUOIA_NULL_OBJECT(VertexData)

protected:
  /// @brief Implementation of `toString` returns stringified members and title
  virtual std::pair<std::string, std::string> toStringImpl() const override;

private:
  /// Allocated VertexBuffer
  std::unique_ptr<NullVertexBuffer> vertexBuffer_;

  /// Allocated IndexBuffer (possibly NULL)
  std::unique_ptr<NullIndexBuffer> indexBuffer_;

private:
  using Base = VertexData;
};

} // namespace render

} // namespace sequoia

#endif
