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

#ifndef SEQUOIA_ENGINE_RENDER_RENDERCOMMAND_H
#define SEQUOIA_ENGINE_RENDER_RENDERCOMMAND_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Render/DrawCommand.h"
#include "sequoia-engine/Render/RenderFwd.h"
#include <vector>

namespace sequoia {

namespace render {

/// @brief Instructions on how to render a list of `DrawCommand`s using the provided
/// `RenderTechnique`s into the given `RenderTarget`
/// @ingroup render
struct SEQUOIA_API RenderCommand {
  RenderCommand(RenderTarget* target) : Target(target) {}

  /// Target to render to (e.g screen or texture)
  RenderTarget* Target = nullptr;

  /// List of techniques to apply
  std::vector<RenderTechnique*> Techniques;

  /// List of draw commands to execute consecutively
  std::vector<DrawCommand*> DrawCommands;

  /// Scene information (e.g lighting information)
  DrawScene* Scene = nullptr;

  /// @brief Reset the command to it's default state and set the new RenderTarget to `target`
  void reset(RenderTarget* target);
};

} // namespace render

} // namespace sequoia

#endif
