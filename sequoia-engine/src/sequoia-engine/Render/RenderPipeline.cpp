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

#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Core/StringUtil.h"
#include "sequoia-engine/Core/Unreachable.h"
#include "sequoia-engine/Render/Program.h"
#include "sequoia-engine/Render/RenderPipeline.h"

namespace sequoia {

namespace render {

static const char* depthFuncToString(RenderPipeline::DepthFuncKind func) {
  switch(func) {
  case RenderPipeline::DepthFuncKind::DF_Never:
    return "Never";
  case RenderPipeline::DepthFuncKind::DF_Less:
    return "Less";
  case RenderPipeline::DepthFuncKind::DF_Equal:
    return "Equal";
  case RenderPipeline::DepthFuncKind::DF_LessEqual:
    return "LessEqual";
  case RenderPipeline::DepthFuncKind::DF_Greater:
    return "Greater";
  case RenderPipeline::DepthFuncKind::DF_NotEqual:
    return "NotEqual";
  case RenderPipeline::DepthFuncKind::DF_GreaterEqual:
    return "GreaterEqual";
  case RenderPipeline::DepthFuncKind::DF_Always:
    return "Always";
  default:
    sequoia_unreachable("invalid DepthFuncKind");
  }
}

template <class T>
std::string printValue(const T& value) {
  return std::to_string(value);
}

template <>
std::string printValue(const bool& value) {
  return value ? "true" : "false";
}

template <>
std::string printValue(const RenderPipeline::DepthFuncKind& value) {
  return depthFuncToString(value);
}

void RenderPipeline::reset() {
#define RENDER_STATE(Type, Name, DefaultValue) this->Name = DefaultValue;
#include "sequoia-engine/Render/RenderState.inc"
#undef RENDER_STATE

  this->Program = nullptr;
}

std::string RenderPipeline::toString() const {
  std::stringstream ss;
  ss << "RenderPipeline[\n";
#define RENDER_STATE(Type, Name, DefaultValue)                                                     \
  ss << "  " #Name " = " << printValue<Type>(Name) << ",\n";
#include "sequoia-engine/Render/RenderState.inc"
#undef RENDER_STATE

  ss << "  Program = " << (Program ? core::indent(Program->toString()) : "null") << ",\n";
  ss << "]";
  return ss.str();
}

} // namespace render

} // namespace sequoia
