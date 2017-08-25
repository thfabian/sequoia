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

#ifndef SEQUOIA_RENDER_FRAMEBUFFER_H
#define SEQUOIA_RENDER_FRAMEBUFFER_H

#include "sequoia/Core/Export.h"
#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Render/RenderFwd.h"
#include "sequoia/Render/RenderSystemObject.h"
#include <memory>
#include <string>

namespace sequoia {

namespace render {

/// @brief Parameters used to construct the frame buffer
/// @ingroup render
struct SEQUOIA_API FrameBufferParameter {
  FrameBufferParameter(int width, int height) : Width(width), Height(height) {}

  FrameBufferParameter(const FrameBufferParameter&) = default;
  FrameBufferParameter(FrameBufferParameter&&) = default;

  /// Width of the frame buffer
  int Width;

  /// Height of the frame buffer
  int Height;

  /// Used multi-sampling
  int MSAA = 0;

  /// @brief Convert to string
  std::string toString() const;
};

/// @brief Frame buffers allow to render to a user-defined location, and thus render without
/// disturbing the main screen.
///
/// @see RenderSystem::createFrameBuffer
/// @ingroup render
class SEQUOIA_API FrameBuffer : public RenderSystemObject, public NonCopyable {
public:
  FrameBuffer(RenderSystemKind kind);
  virtual ~FrameBuffer();

  /// @brief Convert to string
  virtual std::string toString() const = 0;
};

} // namespace render

} // namespace sequoia

#endif
