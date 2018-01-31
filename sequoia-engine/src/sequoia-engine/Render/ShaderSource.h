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

#ifndef SEQUOIA_ENGINE_RENDER_SHADERSOURCE_H
#define SEQUOIA_ENGINE_RENDER_SHADERSOURCE_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/NonCopyable.h"

namespace sequoia {

namespace render {

/// @brief Manager of shader sources
/// @ingroup render
class SEQUOIA_API ShaderSource : public NonCopyable {
public:
  /// @brief RTTI discriminator
  enum ShaderSourceKind { SK_GLSL, SK_HLSL };

  ShaderSource(ShaderSourceKind kind) : kind_(kind) {}

  /// @brief Get the kind of buffer
  ShaderSourceKind getKind() const noexcept { return kind_; }

private:
  ShaderSourceKind kind_;
};

} // namespace render

} // namespace sequoia

#endif
