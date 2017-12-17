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
#include "sequoia-engine/Render/DrawCommand.h"
#include "sequoia-engine/Render/RenderPipeline.h"
#include "sequoia-engine/Render/RenderStateManager.h"
#include "sequoia-engine/Render/Texture.h"
#include "sequoia-engine/Render/VertexData.h"
#include <unordered_set>

namespace sequoia {

namespace render {

RenderStateManager::RenderStateCache() : forceRenderPipelineUpdate_(true), vertexData_(nullptr) {}

void RenderStateManager::reset() {
  forceRenderPipelineUpdate_ = true;
  pipeline_.reset();
  vertexData_ = nullptr;
  textures_.clear();
  uniforms_.clear();
}

void RenderStateManager::resetUniforms(Program* program) {
  auto it = uniforms_.find(program);
  if(it != uniforms_.end())
    it->second.clear();
}

bool RenderStateManager::setRenderPipeline(const RenderPipeline& pipeline) {
  // The program has to be changed first as the textures depend on the currently bound program
  if(pipeline_.Program != pipeline.Program) {
    if(!ProgramChanged(pipeline.Program))
      return false;
    pipeline_.Program = pipeline.Program;
  }

#define RENDER_STATE(Type, Name, DefaultValue)                                                     \
  if(forceRenderPipelineUpdate_ || pipeline_.Name != pipeline.Name) {                              \
    if(!Name##Changed(pipeline.Name))                                                              \
      return false;                                                                                \
    pipeline_.Name = pipeline.Name;                                                                \
  }
#include "sequoia-engine/Render/RenderState.inc"
#undef RENDER_STATE

  forceRenderPipelineUpdate_ = false;
  return true;
}

bool RenderStateManager::setUniformVariable(Program* program, const std::string& name,
                                            const UniformVariable& value) {
  std::unordered_map<std::string, UniformVariable>& oldUniformVariables = uniforms_[program];

  auto it = oldUniformVariables.find(name);
  bool updateNeeded = false;

  if(it == oldUniformVariables.end()) {
    // Variable does not exist -> update
    updateNeeded = true;
  } else {
    if(it->second != value)
      // Variable exists but with different value -> update
      updateNeeded = true;
  }

  if(updateNeeded) {
    if(!UniformVariableChanged(program, name, value))
      return false;
    oldUniformVariables[name] = value;
  }

  return true;
}

bool RenderStateManager::setUniformVariables(
    Program* program, const std::unordered_map<std::string, UniformVariable>& variables) {
  for(const auto& nameVariablePair : variables)
    if(!setUniformVariable(program, nameVariablePair.first, nameVariablePair.second))
      return false;
  return true;
}

bool RenderStateManager::setTextures(const std::unordered_map<int, Texture*>& textures) {
  if(textures_ != textures) {

    // There are 4 possible scenarios for each texture-unit/texture pair
    //
    //  1. Texture unit is bound in `textures_` and requested in `textures` and they share the same
    //     texture -> do nothing
    //  2. Texture unit is bound in `textures_` and requested in `textures` but their textures
    //     differ -> set new texture
    //  3. Texture unit is not bound in `textures_` but requested in `textures`
    //     -> enable the unit and set the texture
    //  4. Texture unit is not bound in `textures_` but not requsted in `textures`-> disable unit
    //
    for(const std::pair<int, Texture*>& texPair : textures) {
      int textureUnit = texPair.first;
      Texture* texture = texPair.second;

      auto it = textures_.find(textureUnit);
      if(it == textures_.end()) {
        // Handle case 3
        if(!TextureChanged(textureUnit, texture, true))
          return false;
      } else {
        if(texture != it->second)
          // Handle case 2
          if(!TextureChanged(textureUnit, texture, true))
            return false;
      }
    }

    for(const std::pair<int, Texture*>& texPair : textures_) {
      if(!textures.count(texPair.first))
        // Handle case 4
        if(!TextureChanged(texPair.first, texPair.second, false))
          return false;
    }

    textures_ = textures;
  }

  return true;
}

bool RenderStateManager::setVertexData(VertexData* vertexData, bool bindForDrawing) {
  if(vertexData_ != vertexData) {
    if(!VertexDataChanged(vertexData, bindForDrawing))
      return false;
    vertexData_ = vertexData;
  }
  return true;
}

std::string RenderStateManager::toString() const {
  return core::format(
      "RenderStateManager["
      "  pipeline = {},\n"
      "  vertexData = {},\n"
      "  textures = {}\n"
      "  uniforms = {}\n"
      "]",
      core::indent(pipeline_.toString()),
      vertexData_ ? core::indent(vertexData_->toString()) : "null",
      textures_.empty() ? "null" : core::indent(core::toStringRange(
                                       textures_,
                                       [](const auto& var) {
                                         return core::format("unit = {},\n"
                                                             "texture = {}\n",
                                                             var.first, var.second->toString());
                                       })),
      uniforms_.empty() ? "null" : core::indent(core::toStringRange(uniforms_, [](const auto& var) {
        Program* program = var.first;
        const auto& uniformMap = var.second;
        return core::format("program = {},\n"
                            "uniforms = {}\n",
                            program->toString(),
                            uniforms_.empty()
                                ? "null"
                                : core::indent(core::toStringRange(uniformMap,
                                                                   [](const auto& var) {
                                                                     return core::format(
                                                                         "name = {},\n"
                                                                         "variable = {}\n",
                                                                         var.first,
                                                                         var.second.toString());
                                                                   }),
                                               4));

      })));
}

} // namespace render

} // namespace sequoia
