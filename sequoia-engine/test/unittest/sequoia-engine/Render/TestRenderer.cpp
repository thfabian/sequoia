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
#include "sequoia-engine/Render/RenderSystem.h"
#include "sequoia-engine/Render/Renderer.h"
#include "sequoia-engine/Unittest/RenderSetup.h"
#include <algorithm>
#include <gtest/gtest.h>
#include <memory>

using namespace sequoia::render;
using namespace sequoia::core;
using namespace sequoia::unittest;

namespace {

class TestRenderer : public Renderer {
  std::vector<std::string> changes_;

protected:
  using Base = Renderer;

#define RENDER_STATE(Type, Name, DefaultValue)                                                     \
  virtual bool Name##Changed(Type value) override {                                                \
    changes_.emplace_back(#Name);                                                                  \
    return true;                                                                                   \
  }
#include "sequoia-engine/Render/RenderState.inc"
#undef RENDER_STATE

  bool ProgramChanged(Program* program) override {
    changes_.emplace_back("Program");
    return true;
  }

  bool VertexDataChanged(VertexData* Data, bool bindForDrawing) override {
    changes_.emplace_back("VertexData_" + std::string(bindForDrawing ? "draw" : "modify"));
    return true;
  }

  bool TextureChanged(int textureUnit, Texture* texture, bool enable) override {
    changes_.emplace_back("Texture" + std::to_string(textureUnit) + "_" +
                          (enable ? "enabled" : "disabled"));
    return true;
  }

  bool UniformVariableChanged(Program* program, const std::string& name,
                              const UniformVariable& value) override {
    changes_.emplace_back("UniformVariable_" + name);
    return true;
  }

  virtual bool ViewportChanged(int x, int y, int width, int height) override {
    changes_.emplace_back("Viewport");
    return true;
  }

  virtual bool clearColorBuffer() override {
    changes_.emplace_back("ColorBuffer");
    return true;
  }

  virtual bool clearDepthBuffer() override {
    changes_.emplace_back("DepthBuffer");
    return true;
  }

  virtual bool clearStencilBuffer() override {
    changes_.emplace_back("StencilBuffer");
    return true;
  }

  virtual bool draw(const DrawCommand* drawCommand) override { return true; }

  virtual std::pair<std::string, std::string> toStringImpl() const override {
    return std::make_pair("TestRenderer",
                          format("{}"
                                 "changes = {}\n",
                                 Base::toStringImpl().second,
                                 changes_.empty() ? "null" : toStringRange(changes_)));
  }

public:
  TestRenderer() : Renderer() {}

  const std::vector<std::string>& getChanges() const { return changes_; }

  void resetChanges() { changes_.clear(); }
};

static std::shared_ptr<VertexData> makeNullVertexData() {
  RenderSystem& rsys = RenderSystem::getSingleton();
  VertexDataParameter param(VertexData::DM_Triangles, VertexLayout{}, 0, 0,
                            Buffer::UH_StaticWriteOnly);
  return rsys.createVertexData(param);
}

SEQUOIA_TESTCASEFIXTURE(RendererTest, RenderSetup);

TEST_F(RendererTest, PipelineChange) {
  RenderSystem& rsys = RenderSystem::getSingleton();

  auto renderer = std::make_unique<TestRenderer>();
  auto program = rsys.createProgram({});

  RenderPipeline pipeline;
  pipeline.Program = program.get();

  // Check *all* change methods are called to set the initial state
  renderer->resetChanges();
  renderer->setRenderPipeline(pipeline);

  {
    const std::vector<std::string>& changes = renderer->getChanges();
#define RENDER_STATE(Type, Name, DefaultValue)                                                     \
  EXPECT_TRUE(std::find(changes.begin(), changes.end(), #Name) != changes.end())                   \
      << "initial state '" #Name "' not set.";
#include "sequoia-engine/Render/RenderState.inc"
#undef RENDER_STATE

    EXPECT_TRUE(std::find(changes.begin(), changes.end(), "Program") != changes.end())
        << "initial state 'Program' not set.";
  }

  // Check that only the correct change function is invoked
  pipeline.DepthTest = !pipeline.DepthTest;

  renderer->resetChanges();
  renderer->setRenderPipeline(pipeline);
  
  {
    const std::vector<std::string>& changes = renderer->getChanges();
    ASSERT_EQ(changes.size(), 1);
    EXPECT_STREQ(changes[0].c_str(), "DepthTest");
  }
}

TEST_F(RendererTest, VertexDataChange) {
  auto renderer = std::make_unique<TestRenderer>();
  auto vertexdata = makeNullVertexData();

  // Bind for drawing
  renderer->resetChanges();
  renderer->setVertexData(vertexdata.get(), true);

  {
    const std::vector<std::string>& changes = renderer->getChanges();    
    ASSERT_EQ(changes.size(), 1);
    EXPECT_STREQ(changes[0].c_str(), "VertexData_draw");
  }
  
  // Rebind same vertex data -> nothing should happen  
  renderer->resetChanges();
  renderer->setVertexData(vertexdata.get(), true);
  ASSERT_EQ(renderer->getChanges().size(), 0);

  // Rebind same vertex data but for modification  
  renderer->resetChanges();
  renderer->setVertexData(vertexdata.get(), false);

  {
    const std::vector<std::string>& changes = renderer->getChanges();    
    ASSERT_EQ(changes.size(), 1);
    EXPECT_STREQ(changes[0].c_str(), "VertexData_modify");
  }
}

TEST_F(RendererTest, RenderBufferChange) {

}

TEST_F(RendererTest, TextureChange) {
  RenderSystem& rsys = RenderSystem::getSingleton();
  
  auto renderer = std::make_unique<TestRenderer>();

  std::unordered_map<int, Texture*> textures;
  auto tex0 = rsys.createTexture(nullptr);
  auto tex1 = rsys.createTexture(nullptr);
  auto tex2 = rsys.createTexture(nullptr);

  // Unit 0 & 1 are enabled
  textures[0] = tex0.get();
  textures[1] = tex1.get();

  renderer->resetChanges();
  renderer->setTextures(textures);

  {
    const std::vector<std::string>& changes = renderer->getChanges();
    ASSERT_EQ(changes.size(), 2);
    EXPECT_TRUE(std::find(changes.begin(), changes.end(), "Texture0_enabled") != changes.end());
    EXPECT_TRUE(std::find(changes.begin(), changes.end(), "Texture1_enabled") != changes.end());
  }

  // Unit 0 & 3 are enabled (requires 1 to be disabled)
  textures[2] = tex2.get();
  textures.erase(1);

  renderer->resetChanges();
  renderer->setTextures(textures);

  {
    const std::vector<std::string>& changes = renderer->getChanges();
    ASSERT_EQ(changes.size(), 2);
    EXPECT_TRUE(std::find(changes.begin(), changes.end(), "Texture1_disabled") != changes.end());
    EXPECT_TRUE(std::find(changes.begin(), changes.end(), "Texture2_enabled") != changes.end());
  }
}

TEST_F(RendererTest, UniformChange) {
  RenderSystem& rsys = RenderSystem::getSingleton();
  
  auto renderer = std::make_unique<TestRenderer>();

  auto program = rsys.createProgram({});
  UniformVariable two(2.0f);
  UniformVariable three(3.0f);
  UniformVariable five(5);

  // five and two are set
  renderer->resetChanges();
  renderer->setUniform(program.get(), "two", two);
  renderer->setUniform(program.get(), "five", five);

  {
    const std::vector<std::string>& changes = renderer->getChanges();
    ASSERT_EQ(changes.size(), 2);
    EXPECT_TRUE(std::find(changes.begin(), changes.end(), "UniformVariable_two") != changes.end());
    EXPECT_TRUE(std::find(changes.begin(), changes.end(), "UniformVariable_five") != changes.end());
  }

  // two changes value (five remains)
  renderer->resetChanges();
  renderer->setUniform(program.get(), "two", three);
  renderer->setUniform(program.get(), "five", five);

  {
    const std::vector<std::string>& changes = renderer->getChanges();
    ASSERT_EQ(changes.size(), 1);
    EXPECT_TRUE(std::find(changes.begin(), changes.end(), "UniformVariable_two") != changes.end());
  }

  // two and five have to be reset as we are going to reset the per program cache
  renderer->resetChanges();

  renderer->resetUniforms(program.get());
  renderer->setUniform(program.get(), "two", three);
  renderer->setUniform(program.get(), "five", five);

  {
    const std::vector<std::string>& changes = renderer->getChanges();
    ASSERT_EQ(changes.size(), 2);
    EXPECT_TRUE(std::find(changes.begin(), changes.end(), "UniformVariable_two") != changes.end());
    EXPECT_TRUE(std::find(changes.begin(), changes.end(), "UniformVariable_five") != changes.end());
  }
}

TEST_F(RendererTest, Draw) {

}

} // anonymous namespace
