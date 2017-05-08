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

#include "sequoia/Render/Camera.h"
#include "sequoia/Render/Viewport.h"

namespace sequoia {

namespace render {

void ViewportListener::viewportGeometryChanged(Viewport* viewport) {}

Viewport::Viewport(Camera* camera, RenderTarget* target, int x, int y, int width, int height)
    : camera_(camera), target_(target), x_(x), y_(y), width_(width), height_(height) {

  // Register the camera as a Viewport listener
  addListener(static_cast<ViewportListener*>(camera_));

  // Notify the camera about the new geometry of the viewport
  for(auto* listener : getListeners<ViewportListener>())
    listener->viewportGeometryChanged(this);
}

Camera* Viewport::getCamera() const { return camera_; }

void Viewport::setCamera(Camera* camera) { camera_ = camera; }

RenderTarget* Viewport::getRenderTarget() const { return target_; }

void Viewport::updateGeometry(int x, int y, int width, int height) {
  x_ = x;
  y_ = y;
  width_ = width;
  height_ = height;

  for(auto* listener : getListeners<ViewportListener>())
    listener->viewportGeometryChanged(this);
}

int Viewport::getX() const { return x_; }

int Viewport::getY() const { return y_; }

int Viewport::getWidth() const { return width_; }

int Viewport::getHeight() const { return height_; }

} // namespace viewport

} // namespace sequoia
