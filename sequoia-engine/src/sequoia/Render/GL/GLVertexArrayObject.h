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

#ifndef SEQUOIA_RENDER_GL_GLVERTEXARRAYOBJECT_H
#define SEQUOIA_RENDER_GL_GLVERTEXARRAYOBJECT_H

#include "sequoia/Render/VertexArrayObject.h"

namespace sequoia {

namespace render {

/// @brief OpenGL vertex array object (VAO) with VBO and EBOs
/// @ingroup gl
class SEQUOIA_RENDER_API GLVertexArrayObject : public VertexArrayObject {
  unsigned int vaoID_, eboID_, vboID_;

  /// Do we draw with indices?
  bool hasIndices_;

  /// Are the buffers allocated?
  bool allocated_;

public:
  virtual ~GLVertexArrayObject();
  GLVertexArrayObject();

  /// @brief Bind the VAO
  void bind();

  /// @brief Unbind the VAO
  void unbind();

  /// @copydoc VertexArrayObject::updateDevice
  void updateDevice() override;

  /// @copydoc VertexArrayObject::attachVertexDataDevice
  void attachVertexDataDevice() override;

  /// @copydoc VertexArrayObject::freeVertexDataDevice
  void freeVertexDataDevice() override;

  /// @brief Do we draw with indices?
  bool hasIndices() const;

  SEQUOIA_GL_OBJECT(VertexArrayObject);
};

} // namespace render

} // namespace sequoia

#endif
