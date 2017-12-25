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

#ifndef SEQUOIA_ENGINE_RENDER_FRAMEBUFFER_H
#define SEQUOIA_ENGINE_RENDER_FRAMEBUFFER_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/NonCopyable.h"
#include "sequoia-engine/Render/RenderFwd.h"
#include "sequoia-engine/Render/RenderSystemObject.h"
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

/// @brief
///
/// https://stackoverflow.com/questions/2213030/whats-the-concept-of-and-differences-between-framebuffer-and-renderbuffer-in-op
///
/// @see RenderSystem::createFrameBuffer
/// @ingroup render
class SEQUOIA_API FrameBuffer : public RenderSystemObject  /*, public Buffer*/ {
public:
  FrameBuffer(RenderSystemKind kind);
  virtual ~FrameBuffer();

  /// @brief Kinds objects which can be attached to a FrameBuffer
  enum AttachmentKind {
    AK_Color,   ///< Color render buffer
    AK_Depth,   ///< Depth render buffer
    AK_Stencil, ///< Stencil render buffer
    AK_Texture  ///< Texture
  };

  /// @brief Convert to string
  virtual std::string toString() const = 0;
};

} // namespace render

} // namespace sequoia

#endif
