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

#ifndef SEQUOIA_GRAPHICS_RENDERSYSTEM_H
#define SEQUOIA_GRAPHICS_RENDERSYSTEM_H

#include "sequoia/Core/Singleton.h"
#include "sequoia/Graphics/Export.h"
#include "sequoia/Graphics/RenderSystemImpl.h"
#include <memory>

namespace sequoia {

namespace graphics {

/// @brief Currently active render-system
/// 
/// The render-system takes care of window and context creation as well as setting up the IO 
/// devices.
/// 
/// @ingroup graphics
class SEQUOIA_GRAPHICS_API RenderSystem : public Singleton<RenderSystem> {
  std::unique_ptr<RenderSystemImpl> renderSystem_;
  
public:
  
  /// @brief Initialize the given render-system
  RenderSystem();
};

} // namespace graphics

} // namespace sequoia

#endif
