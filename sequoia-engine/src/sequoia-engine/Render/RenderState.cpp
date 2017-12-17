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

#include "sequoia-engine/Render/RenderState.h"
#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Core/StringUtil.h"
#include "sequoia-engine/Core/Unreachable.h"
#include "sequoia-engine/Render/FrameBuffer.h"
#include "sequoia-engine/Render/Program.h"
#include "sequoia-engine/Render/Texture.h"
#include "sequoia-engine/Render/VertexData.h"
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

std::string RenderState::toString() const {
  std::stringstream ss;
  ss << "RenderPipeline[\n";
#define RENDER_STATE(Type, Name, DefaultValue)                                                     \
  ss << "  " #Name " = " << printValue<Type>(Name) << ",\n";
#include "sequoia-engine/Render/RenderState.inc"
#undef RENDER_STATE

  ss << "  Program = " << (Program ? core::indent(Program->toString()) : "null") << ",\n";
//  ss << "  VertexArrayObject = " << (VertexData ? core::indent(VertexData->toString()) : "null")
//     << ",\n";
//  ss << "  TextureMap = "
//     << (!TextureMap.empty() ? core::toStringRange(TextureMap,
//                                                   [](const auto& texturePair) {
//                                                     std::stringstream s;
//                                                     s << "unit = " << texturePair.first << "\n";
//                                                     s << "texture = "
//                                                       << texturePair.second->toString();
//                                                     return s.str();
//                                                   })
//                             : "null")
//     << "\n";
  ss << "]";
  return ss.str();
}

//RenderStateCache::~RenderStateCache() {}

//void RenderStateCache::initState() noexcept {
//#define RENDER_STATE(Type, Name, DefaultValue) Name##Changed(state_.Name);
//#include "sequoia-engine/Render/RenderState.inc"
//#undef RENDER_STATE
//}

//bool RenderStateCache::setRenderState(const RenderState& newState) noexcept {
//  // The program has to be changed first as the textures depend on the currently bound program
//  if(state_.Program != newState.Program) {
//    if(!ProgramChanged(newState.Program))
//      return false;
//    state_.Program = newState.Program;
//  }

//#define RENDER_STATE(Type, Name, DefaultValue)                                                     \
//  if(state_.Name != newState.Name) {                                                               \
//    if(!Name##Changed(newState.Name))                                                              \
//      return false;                                                                                \
//    state_.Name = newState.Name;                                                                   \
//  }
//#include "sequoia-engine/Render/RenderState.inc"
//#undef RENDER_STATE

//  if(state_.VertexData != newState.VertexData) {
//    if(!VertexDataChanged(newState.VertexData, true /* always bind for drawing */))
//      return false;
//    state_.VertexData = newState.VertexData;
//  }

//  if(state_.TextureMap != newState.TextureMap) {

//    // There are 4 possible scenarios for each texture unit/texture pair
//    //
//    //  1. Texture unit is bound in state_ and newState and they share the same texture -> nothing
//    //  2. Texture unit is bound in state_ and newState and their texture differs -> set new texture
//    //  3. Texture unit is not bound in state_ but requested in newState -> enable unit/set texture
//    //  4. Texture unit is bound in state_ but not in newState -> disable texture unit
//    //
//    for(const std::pair<int, Texture*>& texPair : newState.TextureMap) {
//      int textureUnit = texPair.first;
//      Texture* texture = texPair.second;

//      auto it = state_.TextureMap.find(textureUnit);
//      if(it == state_.TextureMap.end()) {
//        // Handle case 3
//        if(!TextureChanged(textureUnit, texture, true))
//          return false;
//      } else {
//        if(texture != it->second)
//          // Handle case 2
//          if(!TextureChanged(textureUnit, texture, true))
//            return false;
//      }
//    }

//    for(const std::pair<int, Texture*>& texPair : state_.TextureMap) {
//      if(!newState.TextureMap.count(texPair.first))
//        // Handle case 4
//        if(!TextureChanged(texPair.first, texPair.second, false))
//          return false;
//    }

//    state_.TextureMap = newState.TextureMap;
//  }

//  // Everything ok!
//  return true;
//}

//const RenderState& RenderStateCache::getRenderState() const { return state_; }

//RenderState& RenderStateCache::getRenderState() { return state_; }

//std::string RenderStateCache::toString() const {
//  return core::format("RenderStateCache["
//                      "  state = {}\n"
//                      "]",
//                      core::indent(state_.toString()));
//}

} // namespace render

} // namespace sequoia
