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
#include "sequoia-engine/Math/Math.h"
#include "sequoia-engine/Render/Camera.h"
#include "sequoia-engine/Render/DrawCallContext.h"
#include "sequoia-engine/Render/RenderSystem.h"
#include "sequoia-engine/Render/RenderTechnique.h"
#include "sequoia-engine/Render/Renderer.h"
#include "sequoia-engine/Render/Viewport.h"
#include "sequoia-engine/Unittest/RenderSetup.h"
#include <algorithm>
#include <gtest/gtest.h>
#include <memory>
#include <vector>

using namespace sequoia::render;
using namespace sequoia::core;
using namespace sequoia::unittest;
using namespace sequoia::math;

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

  bool VertexDataChanged(VertexData* Data) override {
    changes_.emplace_back("VertexData");
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

  bool clearRenderBuffers(const std::set<RenderBuffer::RenderBufferKind>& buffersToClear) override {
    for(auto buffer : buffersToClear) {
      switch(buffer) {
      case RenderBuffer::RK_Color:
        changes_.emplace_back("ColorBuffer_cleared");
        break;
      case RenderBuffer::RK_Depth:
        changes_.emplace_back("DepthBuffer_cleared");
        break;
      case RenderBuffer::RK_Stencil:
        changes_.emplace_back("StencilBuffer_cleared");
        break;
      default:
        sequoia_unreachable("invalid RenderBuffer::RenderBufferKind");
      }
    }
    return true;
  }

  virtual bool draw(const DrawCommand& drawCommand) override { return true; }

  virtual std::pair<std::string, std::string> toStringImpl() const override {
    return std::make_pair("TestRenderer",
                          format("{}"
                                 "changes = {}\n",
                                 Base::toStringImpl().second,
                                 changes_.empty() ? "null" : toStringRange(changes_)));
  }

public:
  void forceClearRenderBuffers(const std::set<RenderBuffer::RenderBufferKind>& buffersToClear) {
    clearRenderBuffers(buffersToClear);
  }

  TestRenderer() : Renderer() {}
  const std::vector<std::string>& getChanges() const { return changes_; }
  void resetChanges() { changes_.clear(); }
};

class TestRenderTechnique : public RenderTechnique {
public:
  class TestRenderPass : public RenderPass {
  public:
    void setUp(DrawCallContext& ctx) override {
      // Modify pipeline
      ctx.Pipeline.DepthTest = !ctx.Pipeline.DepthTest;

      // Set some uniforms
      ctx.Uniforms["u_two"] = UniformVariable(2.0f);
      ctx.Uniforms["u_five"] = UniformVariable(5);

      // Don't clear depth buffer
      ctx.BuffersToClear.erase(RenderBuffer::RK_Depth);
    }
    const char* getName() const noexcept override { return "TestRenderPass"; }
  };

  TestRenderTechnique() { pass_ = std::make_unique<TestRenderPass>(); }

  void render(std::function<void(RenderPass*)> renderer) override { renderer(pass_.get()); }

  const char* getName() const noexcept override { return "TestRenderTechnique"; }

private:
  std::unique_ptr<TestRenderPass> pass_;
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
  auto vertexdata0 = makeNullVertexData();
  auto vertexdata1 = makeNullVertexData();

  // Bind for drawing
  renderer->resetChanges();
  renderer->setVertexData(vertexdata0.get());

  {
    const std::vector<std::string>& changes = renderer->getChanges();
    ASSERT_EQ(changes.size(), 1);
    EXPECT_STREQ(changes[0].c_str(), "VertexData");
  }

  // Rebind same vertex data -> nothing should happen
  renderer->resetChanges();
  renderer->setVertexData(vertexdata0.get());
  ASSERT_EQ(renderer->getChanges().size(), 0);

  // Bind another vertex-data
  renderer->resetChanges();
  renderer->setVertexData(vertexdata1.get());

  {
    const std::vector<std::string>& changes = renderer->getChanges();
    ASSERT_EQ(changes.size(), 1);
    EXPECT_STREQ(changes[0].c_str(), "VertexData");
  }
}

TEST_F(RendererTest, RenderBufferChange) {
  auto renderer = std::make_unique<TestRenderer>();

  // Clear all buffers
  std::set<RenderBuffer::RenderBufferKind> buffersToClear = {
      RenderBuffer::RK_Color, RenderBuffer::RK_Depth, RenderBuffer::RK_Stencil};

  renderer->resetChanges();
  renderer->forceClearRenderBuffers(buffersToClear);

  {
    const std::vector<std::string>& changes = renderer->getChanges();
    ASSERT_EQ(changes.size(), 3);
    EXPECT_TRUE(std::find(changes.begin(), changes.end(), "ColorBuffer_cleared") != changes.end());
    EXPECT_TRUE(std::find(changes.begin(), changes.end(), "DepthBuffer_cleared") != changes.end());
    EXPECT_TRUE(std::find(changes.begin(), changes.end(), "StencilBuffer_cleared") !=
                changes.end());
  }

  // Don't clear depth buffer
  buffersToClear.erase(RenderBuffer::RK_Depth);

  renderer->resetChanges();
  renderer->forceClearRenderBuffers(buffersToClear);

  {
    const std::vector<std::string>& changes = renderer->getChanges();
    ASSERT_EQ(changes.size(), 2);
    EXPECT_TRUE(std::find(changes.begin(), changes.end(), "ColorBuffer_cleared") != changes.end());
    EXPECT_TRUE(std::find(changes.begin(), changes.end(), "StencilBuffer_cleared") !=
                changes.end());
  }
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
  renderer->setUniformVariable(program.get(), "two", two);
  renderer->setUniformVariable(program.get(), "five", five);

  {
    const std::vector<std::string>& changes = renderer->getChanges();
    ASSERT_EQ(changes.size(), 2);
    EXPECT_TRUE(std::find(changes.begin(), changes.end(), "UniformVariable_two") != changes.end());
    EXPECT_TRUE(std::find(changes.begin(), changes.end(), "UniformVariable_five") != changes.end());
  }

  // two changes value (five remains)
  renderer->resetChanges();
  renderer->setUniformVariable(program.get(), "two", three);
  renderer->setUniformVariable(program.get(), "five", five);

  {
    const std::vector<std::string>& changes = renderer->getChanges();
    ASSERT_EQ(changes.size(), 1);
    EXPECT_TRUE(std::find(changes.begin(), changes.end(), "UniformVariable_two") != changes.end());
  }

  // two and five have to be reset as we are going to reset the per program cache
  renderer->resetChanges();

  renderer->resetUniforms(program.get());
  renderer->setUniformVariable(program.get(), "two", three);
  renderer->setUniformVariable(program.get(), "five", five);

  {
    const std::vector<std::string>& changes = renderer->getChanges();
    ASSERT_EQ(changes.size(), 2);
    EXPECT_TRUE(std::find(changes.begin(), changes.end(), "UniformVariable_two") != changes.end());
    EXPECT_TRUE(std::find(changes.begin(), changes.end(), "UniformVariable_five") != changes.end());
  }
}

TEST_F(RendererTest, Draw) {
  RenderSystem& rsys = RenderSystem::getSingleton();
  auto renderer = std::make_unique<TestRenderer>();
  auto target = rsys.getMainWindow();

  auto technique = std::make_unique<TestRenderTechnique>();
  auto vertexdata = makeNullVertexData();

  auto camera = std::make_shared<Camera>();
  auto viewport = std::make_shared<Viewport>(target, 0, 0, 80, 80);
  viewport->setCamera(camera.get());
  target->setViewport(viewport);

  DrawCommand drawCmd(vertexdata.get(), mat4(5.0f));

  RenderCommand renderCmd(target);
  renderCmd.Techniques = {technique.get()};
  renderCmd.DrawCommands = {drawCmd};

  renderer->render(renderCmd);
//  std::cout << renderer->toString() << std::endl;
}

} // anonymous namespace
