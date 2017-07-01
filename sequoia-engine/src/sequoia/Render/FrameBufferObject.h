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

#ifndef SEQUOIA_RENDER_FRAMEBUFFEROBJECT_H
#define SEQUOIA_RENDER_FRAMEBUFFEROBJECT_H

#include "sequoia/Core/Export.h"
#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Render/RenderSystemObject.h"
#include <memory>
#include <string>

namespace sequoia {

namespace render {

/// @brief Framebuffer objects allow to render to a user-defined location, and thus render without
/// disturbing the main screen.
///
/// @see RenderSystem::createFrameBufferObject
/// @ingroup render
class SEQUOIA_API FrameBufferObject : public RenderSystemObject, public NonCopyable {
public:
  FrameBufferObject(RenderSystemKind kind);
  virtual ~FrameBufferObject();

  /// @brief Convert to string
  virtual std::string toString() const = 0;
};

} // namespace render

} // namespace sequoia

#endif
