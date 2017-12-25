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

#ifndef SEQUOIA_ENGINE_RENDER_RENDERBUFFER_H
#define SEQUOIA_ENGINE_RENDER_RENDERBUFFER_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/NonCopyable.h"
#include "sequoia-engine/Render/RenderSystemObject.h"
#include <cstdint>

namespace sequoia {

namespace render {

/// @brief A Renderbuffer can be the attached to a FrameBuffer and holds the actual buffer data (an
/// array of bytes, or integers, or pixels).
///
/// The Renderbuffer stores pixel values in native format, so it's optimized for offscreen
/// rendering. In other words, drawing to a Renderbuffer can be much faster than drawing to a
/// texture. The drawback is that pixels uses a native, implementation-dependent format, so that
/// reading from a Renderbuffer is much harder than reading from a texture. Nevertheless, once a
/// Renderbuffer has been painted, one can copy its content directly to screen very quickly using
/// pixel transfer operations.
///
/// @ingroup render
class SEQUOIA_API RenderBuffer : public RenderSystemObject /*, public Buffer*/ {
public:
  /// @brief Different kinds of RenderBuffers
  enum RenderBufferKind {
    RK_Color = 0, ///< Color buffer
    RK_Depth,     ///< Depth buffer
    RK_Stencil,   ///< Stencil buffer
  };
};

} // namespace render

} // namespace sequoia

#endif
