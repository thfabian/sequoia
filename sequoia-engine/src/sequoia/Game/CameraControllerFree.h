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

#include "sequoia/Game/CameraController.h"
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

  CameraControllerFree(const std::string& name, SceneNodeKind kind = SK_CameraControllerFree);
  CameraControllerFree(const CameraControllerFree& other);

  /// @brief Virtual destructor
  virtual ~CameraControllerFree();

  /// @brief Set camera and register as a mouse and key listener
  virtual void setCamera(const std::shared_ptr<render::Camera>& camera) override;

  /// @brief Remove the camera and deregister as a mouse and key listener
  virtual void removeCamera() override;

  /// @brief Check if we move a Camera
  bool isActive() const { return hasCamera(); }

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
};

} // namespace game

} // namespace sequoia
