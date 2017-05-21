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

/// @brief OpenGL vertex array object (VAO)
/// @ingroup gl
class SEQUOIA_RENDER_API GLVertexArrayObject : public VertexArrayObject {

  /// ID of the VAO
  unsigned int id_;

public:
  virtual ~GLVertexArrayObject();
  GLVertexArrayObject();

  SEQUOIA_GL_OBJECT(VertexArrayObject);
};

} // namespace render

} // namespace sequoia

#endif
