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
#include "sequoia-engine/Core/Logging.h"
#include "sequoia-engine/Core/StringUtil.h"
#include "sequoia-engine/Render/Camera.h"
#include "sequoia-engine/Render/DrawCallContext.h"
#include "sequoia-engine/Render/DrawCommand.h"
#include "sequoia-engine/Render/Program.h"
#include "sequoia-engine/Render/RenderCommand.h"
#include "sequoia-engine/Render/RenderPipeline.h"
#include "sequoia-engine/Render/RenderTarget.h"
#include "sequoia-engine/Render/RenderTechnique.h"
#include "sequoia-engine/Render/Renderer.h"
#include "sequoia-engine/Render/Texture.h"
#include "sequoia-engine/Render/VertexData.h"
#include "sequoia-engine/Render/Viewport.h"

namespace sequoia {

namespace render {

namespace {

class Report {
public:
  Report(const char* technique, const char* pass) : technique_(technique), pass_(pass) {}

  template <class... Args>
  void operator()(const char* fmt, Args&&... args) {
    Log::debug("Renderer: {}: {}: {}", technique_, pass_, fmt, std::forward<Args>(args)...);
  }

private:
  const char* technique_;
  const char* pass_;
};
}

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
  // Bind the global FrameBuffer
  // TODO

  Viewport* viewport = command.Target->getViewport();
  SEQUOIA_ASSERT_MSG(viewport, "no Viewport set");
  
  // Render techniques
  for(RenderTechnique* technique : command.Techniques) {
    technique->setUp();
    // TODO: change the interface of technique to have method which is called
    //  render(const std::function<void(RenderPass*)>& renderFunction)

    // Render passes
    for(RenderPass* pass : technique->getPasses()) {
      Report report(technique->getName(), pass->getName());

      DrawCallContext ctx(viewport, command.Target, command.Scene, &command.DrawCommands);
      pass->setUp(ctx);

      // Clear render buffers
      // TODO: make more generic
      if(ctx.ClearColorBuffer)
        clearColorBuffer();
      if(ctx.ClearDepthBuffer)
        clearDepthBuffer();
      if(ctx.ClearStencilBuffer)
        clearStencilBuffer();

      // Update the pipeline (including the program)
      if(!setRenderPipeline(ctx.Pipeline)) {
        report("failed to set RenderPipeline");
        continue;
      }

      if(!setViewport(ctx.Viewport)) {
        report("failed to set Viewport");
        continue;
      }

      auto setUniforms = [&report,
                          this](const std::unordered_map<std::string, UniformVariable>& uniforms) {
        for(const auto& nameVariablePair : uniforms)
          if(!setUniformVariable(this->pipeline_.Program, nameVariablePair.first,
                                 nameVariablePair.second))
            report("failed to set uniform variable '{}'", nameVariablePair.first);
      };

      // Clear all uniforms and set per program uniforms
      resetUniforms(pipeline_.Program);
      setUniforms(ctx.Uniforms);

      Camera* camera = ctx.Viewport->getCamera();

      // Compute projection-view matrix
      math::mat4 matVP = camera->getViewProjectionMatrix();

      // Render the DrawCommands
      for(const DrawCommand* drawCommand : command.DrawCommands) {

        // Set per DrawCommand uniforms
        setUniforms(drawCommand->getUniforms());

        UniformVariable u_matMVP = matVP * drawCommand->getModelMatrix();
        if(!setUniformVariable(this->pipeline_.Program, "u_matMVP", u_matMVP)) {
          report("failed to set UniformVariable 'u_matMVP'");
          continue;
        }

        // Set textures
        if(!setTextures(drawCommand->getTextures())) {
          report("failed to set Textures");
          continue;
        }

        // Set VertexData
        if(!setVertexData(drawCommand->getVertexData())) {
          report("failed to set VertexData");
          continue;
        }

        // Issue the draw command
        if(!draw(drawCommand))
          report("failed to draw DrawCommand");
      }

      pass->tearDown(ctx);
    }

    technique->tearDown();
  }

  // Unbind the global Framebuffer
  // TODO
}

std::string Renderer::toString() const {
  auto stringPair = toStringImpl();
  return core::format("{}[\n  {}]", stringPair.first, core::indent(stringPair.second));
}

std::pair<std::string, std::string> Renderer::toStringImpl() const {
  return std::make_pair(
      "Renderer",
      core::format(
          "pipeline = {},\n"
          "vertexData = {},\n"
          "textures = {},\n"
          "uniforms = {},\n",
          pipeline_.toString(), vertexData_ ? core::indent(vertexData_->toString()) : "null",
          textures_.empty()
              ? "null"
              : core::toStringRange(textures_,
                                    [](const auto& var) {
                                      return core::format("unit = {},\n"
                                                          "  texture = {}",
                                                          var.first,
                                                          core::indent(var.second->toString()));
                                    }),
          uniforms_.empty()
              ? "null"
              : core::indent(core::toStringRange(
                    uniforms_,
                    [this](const auto& programUniformMapPair) {
                      Program* program = programUniformMapPair.first;
                      const auto& uniformMap = programUniformMapPair.second;
                      return core::format(
                          "program = {},\n"
                          "  uniforms = {}",
                          program->toString(),
                          uniforms_.empty()
                              ? "null"
                              : core::toStringRange(uniformMap, [](const auto& nameVariablePair) {
                                  return core::format("name = {},\n"
                                                      "  variable = {}",
                                                      nameVariablePair.first,
                                                      nameVariablePair.second.toString());
                                }));
                    }))

              ));
}

} // namespace render

} // namespace sequoia
