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

#ifndef SEQUOIA_GAME_CAMERACONTROLLERFREE
#define SEQUOIA_GAME_CAMERACONTROLLERFREE

#include "sequoia/Game/CameraController.h"
#include "sequoia/Game/Keymap.h"
#include "sequoia/Render/Input.h"
#include "sequoia/Render/RenderFwd.h"

namespace sequoia {

namespace game {

/// @brief Free-looking camera
/// @ingroup game
class SEQUOIA_GAME_API CameraControllerFree final : public CameraController,
                                                    public MouseListener,
                                                    public KeyListener {
public:
  using Base = CameraController;

  CameraControllerFree(
      const std::string& name, SceneNodeKind kind = SK_CameraControllerFree,
      const std::shared_ptr<Keymap>& forwardKey = Keymap::makeDefault(render::Key_W),
      const std::shared_ptr<Keymap>& backwardKey = Keymap::makeDefault(render::Key_S),
      const std::shared_ptr<Keymap>& leftKey = Keymap::makeDefault(render::Key_A),
      const std::shared_ptr<Keymap>& rightKey = Keymap::makeDefault(render::Key_D),
      const std::shared_ptr<Keymap>& upKey = Keymap::makeDefault(render::Key_Space),
      const std::shared_ptr<Keymap>& downKey = Keymap::makeDefault(render::Key_C));
  CameraControllerFree(const CameraControllerFree& other);

  /// @brief Virtual destructor
  virtual ~CameraControllerFree();

  /// @brief Set camera and register as a mouse and key listener
  virtual void setCamera(const std::shared_ptr<render::Camera>& camera) override;

  /// @brief Remove the camera and deregister as a mouse and key listener
  virtual void removeCamera() override;

  /// @brief Check if we move a Camera
  bool isActive() const { return hasCamera(); }

  /// @brief Update the position of the node
  virtual void update(const UpdateEvent& event) override;

  /// @brief Get/Set the top-speed
  void sertSpeed(float speed) { speed_ = speed; }
  float getSpeed() const { return speed_; }

  /// @brief Clone the scene node and all its children
  virtual std::shared_ptr<SceneNode> clone() override;

  /// @brief RTTI implementation
  static bool classof(const SceneNode* node) noexcept {
    return node->getKind() == SK_CameraControllerFree;
  }

  /// @brief Listener implementations
  /// @{
  void mouseButtonEvent(const render::MouseButtonEvent& event) override;
  void mousePositionEvent(const render::MousePositionEvent& event) override;
  void keyboardEvent(const render::KeyboardEvent& event) override;
  /// @}

protected:
  /// @brief Implementation of `toString` returns stringified members and title
  virtual std::pair<std::string, std::string> toStringImpl() const override;

private:
  std::shared_ptr<Keymap> forwardKey_;  ///< Forward key
  std::shared_ptr<Keymap> backwardKey_; ///< Backward key
  std::shared_ptr<Keymap> leftKey_;     ///< Left key
  std::shared_ptr<Keymap> rightKey_;    ///< Right key
  std::shared_ptr<Keymap> upKey_;       ///< Up key
  std::shared_ptr<Keymap> downKey_;     ///< Down key

  float speed_;
  bool goingForward_ : 1;
  bool goingBack_ : 1;
  bool goingLeft_ : 1;
  bool goingRight_ : 1;
  bool goingUp_ : 1;
  bool goingDown_ : 1;
  bool updateNeeded_ : 1;
};

} // namespace game

} // namespace sequoia

#endif
