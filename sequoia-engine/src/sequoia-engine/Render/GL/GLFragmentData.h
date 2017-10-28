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

#ifndef SEQUOIA_RENDER_GL_GLFRAGMENTDATA_H
#define SEQUOIA_RENDER_GL_GLFRAGMENTDATA_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Render/GL/GLFwd.h"
#include <functional>

namespace sequoia {

namespace render {

/// @brief OpenGL fragement data
///
/// This allows to associate the user-defined varying out variables of the fragment shader.
/// @ingroup gl
struct SEQUOIA_API GLFragmentData {

  /// @brief OpenGL fragment data
  enum Data : unsigned int {
    Color = 0,

    NumAttributes
  };

  /// @brief Get the attachment points from the fragment data
  static GLenum getAttachment(unsigned int data);

  /// @brief Get the name of `data`
  static const char* name(unsigned int data);

  /// @brief Get the enum of the fragement data `name`
  /// @throws RenderSystemException   `name` is not a valid attachment
  static Data data(const char* name);

  /// @brief Check if `name` is a valid fragment data
  static bool isValid(const char* name);

  /// @brief Run `functor` for each fragment data
  static void forEach(std::function<void(unsigned int, const char*)> functor);
};

} // namespace render

} // namespace sequoia

#endif
