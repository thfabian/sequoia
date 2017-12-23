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

#include "sequoia-engine/Core/Compiler.h"
#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Core/Logging.h"
#include "sequoia-engine/Core/StringUtil.h"
#include "sequoia-engine/Render/Camera.h"
#include "sequoia-engine/Render/DrawCallContext.h"
#include "sequoia-engine/Render/DrawCommand.h"
#include "sequoia-engine/Render/Program.h"
#include "sequoia-engine/Render/RenderCommand.h"
#include "sequoia-engine/Render/RenderPipeline.h"
#include "sequoia-engine/Render/RenderSystem.h"
#include "sequoia-engine/Render/RenderTarget.h"
#include "sequoia-engine/Render/RenderTechnique.h"
#include "sequoia-engine/Render/Renderer.h"
#include "sequoia-engine/Render/Texture.h"
#include "sequoia-engine/Render/VertexData.h"
#include "sequoia-engine/Render/Viewport.h"

namespace sequoia {

namespace render {

namespace {

template <class T, bool isPointer>
struct StringifyImpl {
  std::string operator()(const T& value) { return value.toString(); }
};

template <class T>
struct StringifyImpl<T, true> {
  std::string operator()(const T& value) { return value->toString(); }
};

template <class T>
std::string stringify(const T& value) {
  return StringifyImpl<T, std::is_pointer<T>::value>()(value);
}

template <>
std::string stringify(const std::string& value) {
  return value;
}

template <>
std::string stringify(const std::set<RenderBuffer::RenderBufferKind>& buffersToClear) {
  return core::toStringRange(buffersToClear, [](const auto& buffer) {
    switch(buffer) {
    case RenderBuffer::RK_Color:
      return "ColorBuffer";
      break;
    case RenderBuffer::RK_Depth:
      return "DepthBuffer";
      break;
    case RenderBuffer::RK_Stencil:
      return "StencilBuffer";
      break;
    default:
      sequoia_unreachable("invalid RenderBuffer::RenderBufferKind");
    }
  });
}

template <>
std::string stringify(const std::unordered_map<int, Texture*>& textures) {
  return core::toStringRange(textures, [](const auto& var) {
    return core::format("unit = {},\n"
                        "  texture = {}",
                        var.first, core::indent(var.second->toString()));
  });
}

} // anonymous namespace

#define SEQUOIA_PP_CALL_IMPL(r, Data, Elem)                                                        \
  Log::debug("  {} = {}", BOOST_STRINGIZE(Elem), core::indent(stringify(Elem)));

#define SEQUOIA_CALL_IMPL(errorAction, function, ...)                                              \
  if(SEQUOIA_BUILTIN_UNLIKELY(!function(__VA_ARGS__))) {                                           \
    Log::debug("Renderer: {}: {}: failed to call: '" BOOST_STRINGIZE(function) "'",                \
               technique->getName(), pass->getName());                                             \
    if(isDebugEnabled) {                                                                           \
      BOOST_PP_SEQ_FOR_EACH(SEQUOIA_PP_CALL_IMPL, Data, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))     \
    }                                                                                              \
    errorAction;                                                                                   \
  }

#define SEQUOIA_CALL_OR_RETURN(function, ...) SEQUOIA_CALL_IMPL(return, function, __VA_ARGS__)
#define SEQUOIA_CALL_OR_CONTINUE(function, ...) SEQUOIA_CALL_IMPL(continue, function, __VA_ARGS__)

Renderer::Renderer()
    : forceRenderPipelineUpdate_(true), x_(-1), y_(-1), width_(-1), height_(-1),
      vertexData_(nullptr) {}

void Renderer::reset() {
  forceRenderPipelineUpdate_ = true;
  pipeline_.reset();
  vertexData_ = nullptr;
  textures_.clear();
  uniforms_.clear();
  x_ = y_ = width_ = height_ = -1;
}

void Renderer::resetUniforms(Program* program) {
  auto it = uniforms_.find(program);
  if(it != uniforms_.end())
    it->second.clear();
}

bool Renderer::setRenderPipeline(const RenderPipeline& pipeline) {
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

bool Renderer::setUniformVariable(Program* program, const std::string& name,
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

bool Renderer::setTextures(const std::unordered_map<int, Texture*>& textures) {
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

bool Renderer::setVertexData(VertexData* vertexData) {
  if(vertexData_ != vertexData) {
    if(!VertexDataChanged(vertexData))
      return false;
    vertexData_ = vertexData;
  }
  return true;
}

bool Renderer::setViewport(const Viewport* viewport) {
  if(x_ != viewport->getX() || y_ != viewport->getY() || width_ != viewport->getWidth() ||
     height_ != viewport->getHeight()) {
    if(!ViewportChanged(viewport->getX(), viewport->getY(), viewport->getWidth(),
                        viewport->getHeight()))
      return false;
    x_ = viewport->getX();
    y_ = viewport->getY();
    width_ = viewport->getWidth();
    height_ = viewport->getHeight();
  }
  return true;
}

void Renderer::render(const RenderCommand& command) {
  for(RenderTechnique* technique : command.Techniques) {
    auto rendererFun = [&command, &technique, this](RenderPass* pass) -> void {
      bool isDebugEnabled = RenderSystem::getSingleton().getOptions().getBool("Core.Debug");

      Viewport* viewport = command.Target->getViewport();
      SEQUOIA_ASSERT_MSG(viewport, "no Viewport set");

      DrawCallContext ctx(viewport, command.Target, command.Scene, &command.DrawCommands);
      pass->setUp(ctx);

      // Update the pipeline (including the program)
      SEQUOIA_CALL_OR_RETURN(setRenderPipeline, ctx.Pipeline);
      SEQUOIA_CALL_OR_RETURN(setViewport, ctx.Viewport);

      // Clear render buffers
      SEQUOIA_CALL_OR_RETURN(clearRenderBuffers, ctx.BuffersToClear);

      resetUniforms(pipeline_.Program);
      for(const auto& nameVariablePair : ctx.Uniforms) {
        const std::string& name = nameVariablePair.first;
        const UniformVariable& value = nameVariablePair.second;
        SEQUOIA_CALL_OR_CONTINUE(setUniformVariable, pipeline_.Program, name, value);
      }

      Camera* camera = ctx.Viewport->getCamera();
      SEQUOIA_ASSERT_MSG(camera, "no Camera set");

      // Compute projection-view matrix
      math::mat4 matVP = camera->getViewProjectionMatrix();

      // Render the DrawCommands
      for(const DrawCommand& drawCommand : command.DrawCommands) {

        // Set per DrawCommand uniforms
        for(const auto& nameVariablePair : drawCommand.getUniforms()) {
          const std::string& name = nameVariablePair.first;
          const UniformVariable& value = nameVariablePair.second;
          SEQUOIA_CALL_OR_CONTINUE(setUniformVariable, pipeline_.Program, name, value);
        }

        UniformVariable u_matMVP = matVP * drawCommand.getModelMatrix();
        std::string name = "u_matMVP";
        SEQUOIA_CALL_OR_CONTINUE(setUniformVariable, pipeline_.Program, name, u_matMVP);

        // Set textures
        SEQUOIA_CALL_OR_CONTINUE(setTextures, drawCommand.getTextures());

        // Set VertexData
        SEQUOIA_CALL_OR_CONTINUE(setVertexData, drawCommand.getVertexData());

        // Issue the draw command
        SEQUOIA_CALL_OR_CONTINUE(draw, drawCommand);
      }

      pass->tearDown(ctx);
    };

    technique->render(rendererFun);
  }
}

std::string Renderer::toString() const {
  auto stringPair = toStringImpl();
  return core::format("{}[\n  {}]", stringPair.first, core::indent(stringPair.second));
}

std::pair<std::string, std::string> Renderer::toStringImpl() const {
  return std::make_pair(
      "Renderer",
      core::format("pipeline = {},\n"
                   "vertexData = {},\n"
                   "textures = {},\n"
                   "uniforms = {},\n",
                   pipeline_.toString(), vertexData_ ? vertexData_->toString() : "null",
                   textures_.empty()
                       ? "null"
                       : core::toStringRange(textures_,
                                             [](const auto& var) {
                                               return core::format(
                                                   "texture = {{\n"
                                                   "  unit = {},\n"
                                                   "  texture = {}\n"
                                                   "}}",
                                                   var.first, core::indent(var.second->toString()));
                                             }),
                   uniforms_.empty()
                       ? "null"
                       : core::toStringRange(uniforms_, [this](const auto& programUniformMapPair) {
                           Program* program = programUniformMapPair.first;
                           const auto& uniformMap = programUniformMapPair.second;
                           return core::indent(core::format(
                               "perProgramUniforms = {{\n"
                               "  program = {},\n"
                               "  uniforms = {}\n"
                               "}}",
                               core::indent(program->toString()),
                               uniformMap.empty()
                                   ? "null"
                                   : core::indent(core::toStringRange(
                                         uniformMap, [](const auto& nameVariablePair) {
                                           return core::indent(core::format(
                                               "uniform = {{\n"
                                               "  name = {},\n"
                                               "  variable = {}\n"
                                               "}}",
                                               nameVariablePair.first,
                                               core::indent(nameVariablePair.second.toString())));
                                         }))));
                         })));
}

} // namespace render

} // namespace sequoia
