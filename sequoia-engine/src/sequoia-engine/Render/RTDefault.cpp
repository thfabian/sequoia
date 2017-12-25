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

#include "sequoia-engine/Render/DrawCallContext.h"
#include "sequoia-engine/Render/RTDefault.h"
#include "sequoia-engine/Render/RenderSystem.h"

namespace sequoia {

namespace render {

namespace {

class RPDefault : public RenderPass {
  std::shared_ptr<Program> program_;

public:
  RPDefault() {
    auto& rsys = RenderSystem::getSingleton();
    program_ = rsys.createProgram({rsys.createShader(Shader::ST_Vertex, "Default.vert",
                                                     rsys.loadShaderSource("Default.vert")),
                                   rsys.createShader(Shader::ST_Fragment, "Default.frag",
                                                     rsys.loadShaderSource("Default.frag"))});
  }

  void setUp(DrawCallContext& ctx) override { ctx.Pipeline.Program = program_.get(); }

  const char* getName() const noexcept override { return "Default"; }
};

} // anonymous namespace

RTDefault::RTDefault() { defaultPass_ = std::make_unique<RPDefault>(); }

void RTDefault::render(std::function<void(render::RenderPass*)> renderer) {
  renderer(defaultPass_.get());
}

const char* RTDefault::getName() const noexcept { return "Default"; }

} // namespace render

} // namespace sequoia
