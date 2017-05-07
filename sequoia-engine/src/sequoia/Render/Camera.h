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

#ifndef SEQUOIA_RENDER_CAMERA
#define SEQUOIA_RENDER_CAMERA

#include "sequoia/Math/Math.h"
#include "sequoia/Math/Quaternion.h"
#include "sequoia/Render/Export.h"
#include "sequoia/Render/ViewFrustum.h"

namespace sequoia {

namespace render {

/// @brief A viewpoint from which the scene will be rendered
///
/// Sequoia renders scenes from a camera viewpoint into a buffer of some sort, normally a window or
/// a texture (a subclass of RenderTarget). Each camera carries with it a style of rendering, e.g.
/// full textured, flat shaded, wireframe), field of view, rendering distances etc.
///
/// @ingroup render
class Camera : public ViewFrustum {
protected:
  /// Camera orientation
  Quaternionf orientation_;

  /// Camera position
  Vec3f position_;

public:
  Camera() : ViewFrustum() {}

protected:
  void updateFrustum() override = 0;
};

} // namespace render

} // namespace sequoia

#endif
