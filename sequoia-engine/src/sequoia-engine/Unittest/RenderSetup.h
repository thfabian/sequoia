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

#ifndef SEQUOIA_ENGINE_UNITTEST_RENDERSETUP_H
#define SEQUOIA_ENGINE_UNITTEST_RENDERSETUP_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/NonCopyable.h"
#include "sequoia-engine/Unittest/Fixture.h"

namespace sequoia {

namespace unittest {

/// @brief Handle creation and initialization of RenderWindows
/// @ingroup unittest
class SEQUOIA_API RenderSetup : public NonCopyable {
public:
  /// @brief Register and initialize the window
  void SetUp();

  /// @brief Destroy the window
  void TearDown();
};

} // namespace unittest

} // namespace sequoia

#endif
