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
#include <algorithm>
#include <gtest/gtest.h>
#include <memory>

using namespace sequoia::render;

namespace {

class RenderStateCacheTest : public RenderStateCache {
  std::vector<std::string> changes_;

protected:
#define RENDER_STATE(Type, Name, BitfieldWidth, DefaultValue)                                      \
  virtual bool Name##Changed(Type value) override {                                                \
    changes_.emplace_back(#Name);                                                                  \
    return true;                                                                                   \
  }
#include "sequoia/Render/RenderState.inc"
#undef RENDER_STATE

  bool ProgramChanged(Program* program) override {
    changes_.emplace_back("Program");
    return true;
  }

  bool VertexDataChanged(VertexData* Data, bool bindForDrawing) override {
    changes_.emplace_back("VertexArrayObject");
    return true;
  }

  bool TextureChanged(int textureUnit, Texture* texture, bool enable) override {
    changes_.emplace_back("Texture" + std::to_string(textureUnit) + "_" +
                          (enable ? "enabled" : "disabled"));
    return true;
  }

public:
  RenderStateCacheTest() : RenderStateCache() { initState(); }
  const std::vector<std::string>& getChanges() const { return changes_; }
  void resetChanges() { changes_.clear(); }
};

TEST(RenderStateTest, Initialization) {
  // Check *all* change methods are called to set the initial state

  auto stateCache = std::make_unique<RenderStateCacheTest>();
  const std::vector<std::string>& changes = stateCache->getChanges();

#define RENDER_STATE(Type, Name, BitfieldWidth, DefaultValue)                                      \
  EXPECT_TRUE(std::find(changes.begin(), changes.end(), #Name) != changes.end())                   \
      << "initial state \"" #Name "\" not set.";
#include "sequoia/Render/RenderState.inc"
#undef RENDER_STATE
}

TEST(RenderStateTest, StateChange) {
  // Check the correct change function is invoked

  auto stateCache = std::make_unique<RenderStateCacheTest>();
  stateCache->resetChanges();

  RenderState state;
  state.DepthTest = !state.DepthTest;

  stateCache->setRenderState(state);

  const std::vector<std::string>& changes = stateCache->getChanges();
  ASSERT_EQ(changes.size(), 1);
  EXPECT_STREQ(changes[0].c_str(), "DepthTest");
}

TEST(RenderStateTest, TextureChange) {
  auto stateCache = std::make_unique<RenderStateCacheTest>();

  // Unit 0 & 1 are enabled
  RenderState state;
  state.TextureMap[0] = nullptr;
  state.TextureMap[1] = nullptr;

  stateCache->resetChanges();
  stateCache->setRenderState(state);

  {
    const std::vector<std::string>& changes = stateCache->getChanges();
    ASSERT_EQ(changes.size(), 2);
    EXPECT_TRUE(std::find(changes.begin(), changes.end(), "Texture0_enabled") != changes.end());
    EXPECT_TRUE(std::find(changes.begin(), changes.end(), "Texture1_enabled") != changes.end());
  }

  // Unit 0 & 3 are enabled (requires 1 to be disabled)
  state.TextureMap[0] = nullptr;
  state.TextureMap[3] = nullptr;
  state.TextureMap.erase(1);

  stateCache->resetChanges();
  stateCache->setRenderState(state);

  {
    const std::vector<std::string>& changes = stateCache->getChanges();
    ASSERT_EQ(changes.size(), 2);
    EXPECT_TRUE(std::find(changes.begin(), changes.end(), "Texture1_disabled") != changes.end());
    EXPECT_TRUE(std::find(changes.begin(), changes.end(), "Texture3_enabled") != changes.end());
  }
}

} // anonymous namespace
