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

#ifndef SEQUOIA_ENGINE_RENDER_FRAMELISTENER_H
#define SEQUOIA_ENGINE_RENDER_FRAMELISTENER_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Render/RenderFwd.h"
#include <cstddef>

namespace sequoia {

namespace render {

/// @brief Listen to frame events
/// @ingroup render
class SEQUOIA_API FrameListener {
public:
  virtual ~FrameListener() {}

  /// @brief Begin rendering `command`
  virtual void frameListenerRenderingBegin(const RenderCommand& command) = 0;

  /// @brief End of the rendering of `command`
  virtual void frameListenerRenderingEnd(const RenderCommand& command) = 0;
};

} // namespace render

} // namespace sequoia

#endif
