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

#ifndef SEQUOIA_ENGINE_RENDER_GL_GLVERTEXDATA_H
#define SEQUOIA_ENGINE_RENDER_GL_GLVERTEXDATA_H

#include "sequoia-engine/Render/GL/GLFwd.h"
#include "sequoia-engine/Render/GL/GLIndexBuffer.h"
#include "sequoia-engine/Render/GL/GLVertexBuffer.h"
#include "sequoia-engine/Render/VertexData.h"

namespace sequoia {

namespace render {

/// @brief OpenGL Vertex Array
/// @ingroup render
class SEQUOIA_API GLVertexData final : public VertexData {
public:
  /// @brief Allocate a vertex array object (VAO) with a vertex and index buffer
  GLVertexData(const VertexDataParameter& param);

  /// @brief Deallocate all memory
  ~GLVertexData();

  /// @brief Bind the buffer for drawing
  /// @note Do not call this function directly, use `GLStateCacheManager::bindVertexDataForDrawing`
  /// instead.
  void bindForDrawing();

  /// @brief Bind the buffer for modify
  /// @note Do not call this function directly, use `GLStateCacheManager::bindVertexDataForModify`
  /// instead.
  void bindForModify();

  /// @brief Unbind texture
  static void unbind();

  /// @copydoc VertexData::getVertexBuffer
  virtual VertexBuffer* getVertexBuffer() const override { return vertexBuffer_.get(); }

  /// @copydoc VertexData::getIndexBuffer
  virtual IndexBuffer* getIndexBuffer() const override { return indexBuffer_.get(); }

  /// @copydoc VertexData::nextTimestep
  virtual void nextTimestep() override;

  /// @brief Draw the vertex-data
  void draw() const noexcept;

  /// @brief Get the VAO ID
  unsigned int getVAOID() const noexcept { return vaoID_; }

  /// @brief Get the corresponding enum of VertexLayout type
  static GLenum getGLType(VertexLayout2::TypeID type);

  SEQUOIA_GL_OBJECT(VertexData)

protected:
  /// @brief Implementation of `toString` returns stringified members and title
  virtual std::pair<std::string, std::string> toStringImpl() const override;

private:
  /// Allocated VertexBuffer
  std::unique_ptr<GLVertexBuffer> vertexBuffer_;

  /// Allocated IndexBuffer (possibly NULL)
  std::unique_ptr<GLIndexBuffer> indexBuffer_;

  /// Vertex array object (VAO)
  unsigned int vaoID_;

private:
  using Base = VertexData;
};

} // namespace render

} // namespace sequoia

#endif
