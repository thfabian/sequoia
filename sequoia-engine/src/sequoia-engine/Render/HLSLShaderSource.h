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

#ifndef SEQUOIA_ENGINE_RENDER_HLSLSHADERSOURCE_H
#define SEQUOIA_ENGINE_RENDER_HLSLSHADERSOURCE_H

#include "sequoia-engine/Render/ShaderSource.h"

namespace sequoia {

namespace render {

/// @brief HLSL shader source wrapper
/// @ingroup render
class SEQUOIA_API HLSLShaderShource : public ShaderSource {
public:
  HLSLShaderShource(ShaderSource::ShaderSourceKind kind);

  static bool classof(const ShaderSource* source) {
    return source->getKind() == SK_HLSL;
  }
};

} // namespace render

} // namespace sequoia

#endif
