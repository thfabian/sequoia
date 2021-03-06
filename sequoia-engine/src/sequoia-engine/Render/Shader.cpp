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

#include "sequoia-engine/Render/Shader.h"
#include "sequoia-engine/Core/Unreachable.h"

namespace sequoia {

namespace render {

Shader::~Shader() {}

Shader::Shader(RenderSystemKind kind, Shader::ShaderType type)
    : RenderRessource(kind), type_(type) {}

const char* Shader::shaderTypeToString(Shader::ShaderType type) {
  switch(type) {
  case ST_Compute:
    return "Compute";
  case ST_Vertex:
    return "Vertex";
  case ST_TessControl:
    return "TessControl";
  case ST_TessEvaluation:
    return "TessEvaluation";
  case ST_Geometry:
    return "Geometry";
  case ST_Fragment:
    return "Fragment";
  default:
    sequoia_unreachable("invalid type");
  }
}

} // namespace render

} // namespace sequoia
