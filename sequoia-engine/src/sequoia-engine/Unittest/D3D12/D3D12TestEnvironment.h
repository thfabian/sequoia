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

#ifndef SEQUOIA_ENGINE_UNITTEST_D3D12_D3D12TESTENVIRONMENT_H
#define SEQUOIA_ENGINE_UNITTEST_D3D12_D3D12TESTENVIRONMENT_H

#include "sequoia-engine/Render/D3D12/D3D12RenderSystem.h"
#include "sequoia-engine/Unittest/TestEnvironment.h"
#include <gtest/gtest.h>

namespace sequoia {

namespace unittest {

/// @brief D3D12 test environment
/// @ingroup unittest
class SEQUOIA_API D3D12TestEnvironment : public TestEnvironment {
  std::unique_ptr<render::RenderSystem> renderSystem_;

public:
  D3D12TestEnvironment(int argc, char* argv[]);

  /// @brief Create the D3D12 RenderSystem
  virtual void SetUp() override;

  /// @brief Tear-down the D3D12 RenderSystem
  virtual void TearDown() override;
};

} // namespace unittest

} // namespace sequoia

#endif
