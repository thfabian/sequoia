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

#ifndef SEQUOIA_GAME_CAMERACONTROLLER
#define SEQUOIA_GAME_CAMERACONTROLLER

#include "sequoia/Game/SceneNode.h"
#include "sequoia/Render/Camera.h"
#include "sequoia/Render/RenderFwd.h"

namespace sequoia {

namespace game {

/// @brief Control a camera
/// @ingroup game
class SEQUOIA_GAME_API CameraController : public SceneNode {
public:
  using Base = SceneNode;

  CameraController(const std::string& name, SceneNodeKind kind = SK_CameraController);
  CameraController(const CameraController& other);

  /// @brief Virtual destructor
  virtual ~CameraController();

  /// @brief Set the camera of the node
  virtual void setCamera(const std::shared_ptr<render::Camera>& camera);

  /// @brief Get the attached camera
  const std::shared_ptr<render::Camera>& getCamera() const { return camera_; }

  /// @brief Remove the attached camera
  virtual void removeCamera() { camera_ = nullptr; }

  /// @brief Check if the node has a camera attached
  bool hasCamera() const { return camera_ != nullptr; }

  /// @brief Clone the scene node and all its children
  virtual std::shared_ptr<SceneNode> clone() override;

  /// @brief RTTI implementation
  static bool classof(const SceneNode* node) noexcept {
    return node->getKind() >= SK_CameraController && node->getKind() < SK_CameraControllerLast;
  }

protected:
  /// @brief Implementation of `toString` returns stringified members and title
  virtual std::pair<std::string, std::string> toStringImpl() const override;

private:
  /// Camera to be controlled
  std::shared_ptr<render::Camera> camera_;
};

} // namespace game

} // namespace sequoia

#endif
