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

#ifndef SEQUOIA_ENGINE_RENDER_GL_GLPIXELFORMAT_H
#define SEQUOIA_ENGINE_RENDER_GL_GLPIXELFORMAT_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Render/GL/GLFwd.h"
#include <unordered_map>

namespace sequoia {

namespace render {

/// @brief Pixel storage modes
/// @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glPixelStore.xhtml
class SEQUOIA_API GLPixelFormat {
  friend class GLRenderer;

  /// Map of the the parameter of the pixel format to their respective values
  std::unordered_map<GLenum, int> format_;

public:
  /// @brief Get the value of `param`
  int get(GLenum param) const noexcept;

  /// @brief Set the `value` of `param`
  void set(GLenum param, int value) noexcept;

  /// @brief Convert pixel format to string
  std::string toString() const;
};

} // namespace render

} // namespace sequoia

#endif
