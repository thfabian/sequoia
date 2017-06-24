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

#include "sequoia/Render/RenderState.h"
#include "sequoia/Core/Format.h"
#include "sequoia/Core/StringUtil.h"
#include "sequoia/Core/Unreachable.h"
#include "sequoia/Render/Program.h"
#include "sequoia/Render/Texture.h"
#include "sequoia/Render/VertexArrayObject.h"
#include <unordered_set>

namespace sequoia {

namespace render {

static const char* depthFuncToString(RenderState::DepthFuncKind func) {
  switch(func) {
  case RenderState::DepthFuncKind::DF_Never:
    return "Never";
  case RenderState::DepthFuncKind::DF_Less:
    return "Less";
  case RenderState::DepthFuncKind::DF_Equal:
    return "Equal";
  case RenderState::DepthFuncKind::DF_LessEqual:
    return "LessEqual";
  case RenderState::DepthFuncKind::DF_Greater:
    return "Greater";
  case RenderState::DepthFuncKind::DF_NotEqual:
    return "NotEqual";
  case RenderState::DepthFuncKind::DF_GreaterEqual:
    return "GreaterEqual";
  case RenderState::DepthFuncKind::DF_Always:
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
std::string printValue(const RenderState::DepthFuncKind& value) {
  return depthFuncToString(value);
}

RenderState::RenderState() {
#define RENDER_STATE(Type, Name, BitfieldWidth, DefaultValue) this->Name = DefaultValue;
#include "sequoia/Render/RenderState.inc"
#undef RENDER_STATE

  Program = nullptr;
  VertexArrayObject = nullptr;
}

std::string RenderState::toString() const {
  std::stringstream ss;
  ss << "RenderState[\n";
#define RENDER_STATE(Type, Name, BitfieldWidth, DefaultValue)                                      \
  ss << "  " #Name " = " << printValue<Type>(Name) << ",\n";
#include "sequoia/Render/RenderState.inc"
#undef RENDER_STATE

  ss << "  Program = " << (Program ? core::indent(Program->toString()) : "null") << ",\n";
  ss << "  VertexArrayObject = "
     << (VertexArrayObject ? core::indent(VertexArrayObject->toString()) : "null") << "\n";
  ss << "  TextureMap = "
     << (!TextureMap.empty() ? core::toStringRange(TextureMap,
                                                   [](const auto& texturePair) {
                                                     std::stringstream s;
                                                     s << "unit = " << texturePair.first << "\n";
                                                     s << "texture = "
                                                       << texturePair.second->toString();
                                                     return s.str();
                                                   })
                             : "null")
     << "\n";
  ss << "]";
  return ss.str();
}

RenderStateCache::~RenderStateCache() {}

void RenderStateCache::initState() noexcept {
#define RENDER_STATE(Type, Name, BitfieldWidth, DefaultValue) Name##Changed(state_.Name);
#include "sequoia/Render/RenderState.inc"
#undef RENDER_STATE
}

void RenderStateCache::setRenderState(const RenderState& state) noexcept {
#define RENDER_STATE(Type, Name, BitfieldWidth, DefaultValue)                                      \
  if(state_.Name != state.Name) {                                                                  \
    Name##Changed(state.Name);                                                                     \
    state_.Name = state.Name;                                                                      \
  }
#include "sequoia/Render/RenderState.inc"
#undef RENDER_STATE

  if(state_.Program != state.Program) {
    ProgramChanged(state.Program);
    state_.Program = state.Program;
  }

  if(state_.VertexArrayObject != state.VertexArrayObject) {
    VertexArrayObjectChanged(state.VertexArrayObject);
    state_.VertexArrayObject = state.VertexArrayObject;
  }

  for(const std::pair<int, Texture*>& texPair : state.TextureMap)
    if(!state_.TextureMap.count(texPair.first) ||
       state_.TextureMap[texPair.first] != texPair.second)
      TextureChanged(texPair.first, texPair.second);

  state_.TextureMap = state.TextureMap;
}

const RenderState& RenderStateCache::getRenderState() const { return state_; }

std::string RenderStateCache::toString() const {
  return core::format("RenderStateCache["
                      "  state = %s\n"
                      "]",
                      core::indent(state_.toString()));
}

} // namespace render

} // namespace sequoia
