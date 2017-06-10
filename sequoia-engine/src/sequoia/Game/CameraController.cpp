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

#include "sequoia/Core/Format.h"
#include "sequoia/Game/CameraController.h"
#include "sequoia/Game/SceneGraph.h"
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
}

std::shared_ptr<SceneNode> CameraController::clone() {
  return SceneGraph::create<CameraController>(*this);
}

std::pair<std::string, std::string> CameraController::toStringImpl() const {
  return std::make_pair("CameraController", core::format("%s"
                                                         "camera = %s,\n",
                                                         Base::toStringImpl().second,
                                                         camera_ ? camera_->toString() : "null"));
}

} // namespace game

} // namespace sequoia
