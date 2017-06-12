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

#ifndef SEQUOIA_UNITTEST_GL_GLRENDERTEST_H
#define SEQUOIA_UNITTEST_GL_GLRENDERTEST_H

#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Render/GL/GLRenderWindow.h"
#include "sequoia/Unittest/Export.h"
#include <gtest/gtest.h>

namespace sequoia {

namespace unittest {

/// @brief Handle creation and initialization of OpenGL RenderWindows for unittesting
/// @ingroup unittest
class SEQUOIA_UNITTEST_API GLRenderTest : public testing::Test, public NonCopyable {
  render::GLRenderWindow* window_;

protected:
  /// @brief Get the active window
  render::GLRenderWindow* getWindow() const;

  /// @brief Register and initialize the window
  void SetUp();

  /// @brief Destroy the window
  void TearDown();
};

} // namespace unittest

} // namespace sequoia

#endif
