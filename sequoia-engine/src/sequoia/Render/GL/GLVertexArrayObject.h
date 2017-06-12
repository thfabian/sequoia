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

#ifndef SEQUOIA_RENDER_GL_GLVERTEXARRAYOBJECT_H
#define SEQUOIA_RENDER_GL_GLVERTEXARRAYOBJECT_H

#include "sequoia/Render/VertexArrayObject.h"

namespace sequoia {

namespace render {

/// @brief OpenGL vertex array object (VAO) with VBO and EBOs
/// @ingroup gl
class SEQUOIA_RENDER_API GLVertexArrayObject : public VertexArrayObject {
  /// OpenGL ids
  unsigned int vaoID_, eboID_, vboID_;

  /// Are the buffers allocated?
  bool allocated_;

public:
  virtual ~GLVertexArrayObject();
  GLVertexArrayObject();

  /// @brief Bind the VAO
  void bind();

  /// @brief Get the VAO ID
  unsigned int getVAOID() const;

  /// @copydoc VertexArrayObject::updateVertexData
  virtual void writeVertexData(std::size_t offset, std::size_t length) override;

  /// @copydoc VertexArrayObject::updateIndexData
  virtual void writeIndexData(std::size_t offset, std::size_t length) override;

  /// @copydoc VertexArrayObject::attachVertexDataDevice
  void attachVertexDataDevice() override;

  /// @copydoc VertexArrayObject::freeVertexDataDevice
  void freeVertexDataDevice() override;

  /// @copydoc VertexArrayObject::toString
  std::string toString() const override;

  SEQUOIA_GL_OBJECT(VertexArrayObject);

private:
  std::size_t getNumVertexBytes(std::size_t length) const;
  std::size_t getNumIndexBytes(std::size_t length) const;
};

} // namespace render

} // namespace sequoia

#endif
