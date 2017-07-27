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

#ifndef SEQUOIA_UNITTEST_TESTOPTIONS_H
#define SEQUOIA_UNITTEST_TESTOPTIONS_H

#include "sequoia/Core/Options.h"
#include "sequoia/Core/Singleton.h"
#include <stack>

namespace sequoia {

namespace unittest {

/// @brief Singleton wrapper to for @ref sequoia::core::Options "Options"
/// @ingroup unittest
class SEQUOIA_API TestOptions : public Options, public Singleton<TestOptions> {
public:
  TestOptions();

  /// @brief Take a snapshot of the currently set options
  void save();

  /// @brief Load the options from the most recently stored snapshot
  void load();

  /// @brief Remove all but the first stored snapshot and load it
  void restoreFirstSnapshot();

private:
  std::stack<Options> snapshots_;
};

} // namespace unittest

} // namespace sequoia

#endif
