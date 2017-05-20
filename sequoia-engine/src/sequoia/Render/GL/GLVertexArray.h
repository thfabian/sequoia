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

#ifndef SEQUOIA_RENDER_GL_GLVERTEXARRAY_H
#define SEQUOIA_RENDER_GL_GLVERTEXARRAY_H

#include "sequoia/Render/VertexArray.h"

namespace sequoia {

namespace render {

/// @brief OpenGL hardware buffer
/// @ingroup gl
class SEQUOIA_RENDER_API GLVertexArray : public VertexArray {

  /// ID of the Vertex Array Object (VAO)
  unsigned int id_;

public:
  virtual ~GLVertexArray();
  GLVertexArray();

  SEQUOIA_GL_OBJECT(VertexArray);
};

} // namespace render

} // namespace sequoia

#endif
