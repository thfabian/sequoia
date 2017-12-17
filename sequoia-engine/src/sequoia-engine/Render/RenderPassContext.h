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

#ifndef SEQUOIA_ENGINE_RENDER_RENDERPASSCONTEXT_H
#define SEQUOIA_ENGINE_RENDER_RENDERPASSCONTEXT_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Render/RenderFwd.h"

namespace sequoia {

namespace render {

/// @brief Immutable information passed to each RenderPass during execution of the pass
/// @ingroup render
struct RenderPassContext {
  DrawScene* Scene; ///< Scene information (e.g lighting)
};

} // namespace render

} // namespace sequoia

#endif
