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

#ifndef SEQUOIA_RENDER_RENDERSYSTEM_H
#define SEQUOIA_RENDER_RENDERSYSTEM_H

#include "sequoia/Core/Singleton.h"
#include "sequoia/Render/Export.h"
#include "sequoia/Render/RenderSystemImpl.h"
#include <memory>

namespace sequoia {

namespace render {

/// @brief Currently active render-system
///
/// The render-system takes care of window and context creation as well as setting up the IO
/// devices. Note that there can only be one RenderSystem active at the time.
///
/// @ingroup render
class SEQUOIA_RENDER_API RenderSystem : public Singleton<RenderSystem> {
  std::unique_ptr<RenderSystemImpl> renderSystem_;

public:
  /// @brief Initialize the render-system
  RenderSystem();
};

} // namespace render

} // namespace sequoia

#endif
