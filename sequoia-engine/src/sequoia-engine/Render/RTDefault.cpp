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

#include "sequoia-engine/Render/RTDefault.h"
#include "sequoia-engine/Render/RenderSystem.h"

namespace sequoia {

namespace render {

namespace {

class RPDefault : public RenderPass {
  std::shared_ptr<Program> program_;

public:
  RPDefault() {
    //    const auto& rsys = RenderSystem::getSingleton();
    //    auto vertexShader = rsys.createShader(Shader::ST_Vertex, rsys.loadShaderSource("Default.vert"));
  }

  void setUp(DrawCallContext& ctx) override {}

  const char* getName() const noexcept override { return "RPDefault"; }
};

} // anonymous namespace

RTDefault::RTDefault()
{
  
}

void RTDefault::render(std::function<void(render::RenderPass*)> renderer) {
  renderer(defaultPass_.get());
}

const char* RTDefault::getName() const  noexcept { return "RTDefault"; }

} // namespace render

} // namespace sequoia
