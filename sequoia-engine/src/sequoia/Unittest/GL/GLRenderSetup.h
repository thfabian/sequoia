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

#ifndef SEQUOIA_UNITTEST_GL_GLRENDERSETUP_H
#define SEQUOIA_UNITTEST_GL_GLRENDERSETUP_H

#include "sequoia/Core/Export.h"
#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Render/GL/GLRenderWindow.h"
#include "sequoia/Unittest/Fixture.h"

namespace sequoia {

namespace unittest {

/// @brief Handle creation and initialization of OpenGL RenderWindows
/// @ingroup unittest
class SEQUOIA_API GLRenderSetup : public NonCopyable {
public:
  /// @brief Register and initialize the window
  void SetUp();

  /// @brief Destroy the window
  void TearDown();
};

#define SEQUOIA_RENDER_TEST_FIXTURE(Name)                                                          \
  SEQUOIA_TEST_FIXTURE(Name, sequoia::unittest::GLRenderSetup)
#define SEQUOIA_RENDER_BENCHMARK_FIXTURE(Name)                                                     \
  SEQUOIA_BENCHMARK_FIXTURE(Name, sequoia::unittest::GLRenderSetup)

} // namespace unittest

} // namespace sequoia

#endif
