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

#ifndef SEQUOIA_RENDER_GL_GLVIEWFRUSTUM_H
#define SEQUOIA_RENDER_GL_GLVIEWFRUSTUM_H

#include "sequoia/Render/Camera.h"

namespace sequoia {

namespace render {

/// @brief OpenGL implementation of the Camera
/// @ingroup gl
class SEQUOIA_RENDER_API GLCamera : public Camera {
  
protected:
  void updateFrustum();
};

} // namespace render

} // namespace sequoia

#endif
