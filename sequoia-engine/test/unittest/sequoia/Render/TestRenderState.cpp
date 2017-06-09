//===--------------------------------------------------------------------------------*- C++ -*-===//
//                         _____                        _
//                        / ____|                      (_)
//                       | (___   ___  __ _ _   _  ___  _  __ _
//                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
//                        ____) |  __/ (_| | |_| | (_) | | (_| |
//                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine
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

using namespace sequoia::render;

namespace {

class RenderStateCacheTest : public RenderStateCache {
  std::vector<std::string> changes_;

protected:
#define RENDER_STATE(Type, Name, BitfieldWidth, DefaultValue)                                      \
  virtual void Name##Changed(Type value) override { changes_.emplace_back(#Name); }
#include "sequoia/Render/RenderState.inc"
#undef RENDER_STATE

  void ProgramChanged(Program* program) override { changes_.emplace_back("Program"); }

  void VertexArrayObjectChanged(VertexArrayObject* vao) override {
    changes_.emplace_back("VertexArrayObject");
  }

public:
  RenderStateCacheTest() : RenderStateCache() { initState(); }
  const std::vector<std::string>& getChanges() const { return changes_; }
  void clear() { changes_.clear(); }
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
  stateCache->clear();

  RenderState state;
  state.DepthTest = !state.DepthTest;

  stateCache->setRenderState(state);

  const std::vector<std::string>& changes = stateCache->getChanges();
  EXPECT_EQ(changes.size(), 1);
  EXPECT_STREQ(changes[0].c_str(), "DepthTest");
}

} // anonymous namespace
