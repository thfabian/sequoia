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
#include "sequoia/Game/CameraControllerFree.h"
#include "sequoia/Game/Game.h"
#include "sequoia/Game/SceneGraph.h"

namespace sequoia {

namespace game {

CameraControllerFree::CameraControllerFree(const std::string& name, SceneNode::SceneNodeKind kind)
    : Base(name, kind) {}

CameraControllerFree::CameraControllerFree(const CameraControllerFree& other) : Base(other) {}

CameraControllerFree::~CameraControllerFree() {}

void CameraControllerFree::setCamera(const std::shared_ptr<render::Camera>& camera) {
  Base::setCamera(camera);
  Game::getSingleton().addListener(static_cast<MouseListener*>(this));
  Game::getSingleton().addListener(static_cast<KeyListener*>(this));
}

void CameraControllerFree::removeCamera() {
  Base::removeCamera();
  Game::getSingleton().removeListener(static_cast<MouseListener*>(this));
  Game::getSingleton().removeListener(static_cast<KeyListener*>(this));
}

std::shared_ptr<SceneNode> CameraControllerFree::clone() {
  return SceneGraph::create<CameraControllerFree>(*this);
}

void CameraControllerFree::keyboardEvent(const render::KeyboardEvent& event) {
  if(!hasCamera())
    return;
}

void CameraControllerFree::mouseButtonEvent(const render::MouseButtonEvent& event) {
  if(!hasCamera())
    return;
}

void CameraControllerFree::mousePositionEvent(const render::MousePositionEvent& event) {
  if(!hasCamera())
    return;
}

std::pair<std::string, std::string> CameraControllerFree::toStringImpl() const {
  return std::make_pair("CameraControllerFree", core::format("%s", Base::toStringImpl().second));
}

} // namespace game

} // namespace sequoia
