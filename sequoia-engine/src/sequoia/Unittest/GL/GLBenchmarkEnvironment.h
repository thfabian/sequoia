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

#ifndef SEQUOIA_UNITTEST_GL_GLBENCHMARKENVIRONMENT_H
#define SEQUOIA_UNITTEST_GL_GLBENCHMARKENVIRONMENT_H

#include "sequoia/Render/GL/GLRenderSystem.h"
#include "sequoia/Unittest/BenchmarkEnvironment.h"

namespace sequoia {

namespace unittest {

/// @brief OpenGL benchmark environment
/// @ingroup unittest
class SEQUOIA_API GLBenchmarkEnvironment : public BenchmarkEnvironment {
  std::unique_ptr<render::RenderSystem> renderSystem_;

public:
  GLBenchmarkEnvironment(int argc, char* argv[]);

  /// @brief Create the OpenGL RenderSystem
  virtual void SetUp() override;

  /// @brief Tear-down the OpenGL RenderSystem
  virtual void TearDown() override;
};

} // namespace unittest

} // namespace sequoia

#endif
