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

#ifndef SEQUOIA_EXAMPLES_SANDBOX_SANDBOXSCENE_H
#define SEQUOIA_EXAMPLES_SANDBOX_SANDBOXSCENE_H

#include "sequoia-engine/Game/Scene.h"
#include "sequoia-engine/Render/RTDefault.h"
#include <memory>

using namespace sequoia;

namespace sequoia_examples {

namespace sandbox {

/// @brief Sandbox example scene
/// @ingroup sandbox
class SandboxScene final : public game::Scene {

  /// RenderTechniques
  std::unique_ptr<render::RTDefault> rtDefault_;

public:
  /// @brief Initalize the Scene
  SandboxScene(const std::string& name);

  /// @copydoc Scene::update
  virtual void update() override;

  /// @brief Scene::prepareRenderTechniques
  virtual void prepareRenderTechniques(std::vector<render::RenderTechnique*>& techiques) override;
};

} // namespace sandbox

} // namespace sequoia_examples

#endif
