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

#include "sequoia/Game/CameraController.h"
#include "sequoia/Core/Format.h"
#include "sequoia/Render/Camera.h"

namespace sequoia {

namespace game {

CameraController::CameraController(const std::string& name, SceneNode::SceneNodeKind kind)
    : Base(name, kind) {}

CameraController::CameraController(const CameraController& other)
    : Base(other), camera_(other.camera_) {}

CameraController::~CameraController() {}

void CameraController::setCamera(const std::shared_ptr<render::Camera>& camera) {
  camera_ = camera;
  setPosition(camera->getPosition());
  setOrientation(camera->getOrientation());
  camera_->addListener<render::CameraPositionListener>(this);
}

void CameraController::removeCamera() {
  camera_->removeListener<render::CameraPositionListener>(this);
  camera_ = nullptr;
}

void CameraController::update(const SceneNode::UpdateEvent& event) {
  Base::update(event);
  if(hasCamera()) {
    camera_->setPosition(getPosition());
    camera_->setOrientation(getOrientation());
  }
}

std::shared_ptr<SceneNode> CameraController::clone() {
  return SceneNode::allocate<CameraController>(*this);
}

void CameraController::cameraListenerPositionChanged(render::Camera* camera) {
  setPosition(camera->getPosition());
}

void CameraController::cameraListenerRotationChanged(render::Camera* camera) {
  setOrientation(camera->getOrientation());
}

std::pair<std::string, std::string> CameraController::toStringImpl() const {
  return std::make_pair("CameraController", core::format("%s"
                                                         "camera = %s,\n",
                                                         Base::toStringImpl().second,
                                                         camera_ ? camera_->toString() : "null"));
}

} // namespace game

} // namespace sequoia
