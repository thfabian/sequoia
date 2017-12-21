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

#ifndef SEQUOIA_ENGINE_UNITTEST_NULL_NULLTESTENVIRONMENT_H
#define SEQUOIA_ENGINE_UNITTEST_NULL_NULLTESTENVIRONMENT_H

#include "sequoia-engine/Render/Null/NullRenderSystem.h"
#include "sequoia-engine/Unittest/TestEnvironment.h"
#include <gtest/gtest.h>

namespace sequoia {

namespace unittest {

/// @brief Null test environment
/// @ingroup unittest
class SEQUOIA_API NullTestEnvironment : public TestEnvironment {
  std::unique_ptr<render::RenderSystem> renderSystem_;

public:
  NullTestEnvironment(int argc, char* argv[]);

  /// @brief Create the Null RenderSystem
  virtual void SetUp() override;

  /// @brief Tear-down the Null RenderSystem
  virtual void TearDown() override;
};

} // namespace unittest

} // namespace sequoia

#endif
