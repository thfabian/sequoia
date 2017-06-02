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

#ifndef SEQUOIA_RENDER_GL_GLVERTEXATTRIBUTE_H
#define SEQUOIA_RENDER_GL_GLVERTEXATTRIBUTE_H

#include "sequoia/Render/Export.h"
#include <functional>

namespace sequoia {

namespace render {

/// @brief OpenGL vertex attributes
/// @ingroup gl
struct SEQUOIA_RENDER_API GLVertexAttribute {
  
  /// @brief OpenGL ids of known vertex attributes
  enum Attribute : unsigned int {
    Position = 0,
    Normal,
    TexCoord,
    Color,
    Tangent,
    Bitangent,

    NumAttributes
  };

  /// @brief Get the name of `attribute`
  static const char* name(Attribute attribute);

  /// @brief Get the enum of the vertex attribute `name`
  /// @throws RenderSystemException   `name` is not a valid attribute
  static Attribute attribute(const char* name);

  /// @brief Check if `name` is a valid attribute
  static bool isValid(const char* name);

  /// @brief Run `functor` for each attribute
  static void forEach(std::function<void(unsigned int, const char*)> functor);
};

} // namespace render

} // namespace sequoia

#endif
