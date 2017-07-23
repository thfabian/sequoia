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

#ifndef SEQUOIA_UNITTEST_BENCHMARKENVIRONMENT_H
#define SEQUOIA_UNITTEST_BENCHMARKENVIRONMENT_H

#include "sequoia/Core/Export.h"
#include "sequoia/Core/Singleton.h"
#include "sequoia/Core/SingletonManager.h"
#include <memory>

namespace sequoia {

namespace unittest {

/// @brief Global benchmark environment
/// @ingroup unittest
class SEQUOIA_API BenchmarkEnvironment : public Singleton<BenchmarkEnvironment> {
public:
  /// @brief Parse command-line
  BenchmarkEnvironment(int argc, char* argv[]);

  /// @brief Virtual destructor
  virtual ~BenchmarkEnvironment();

  /// @brief Set up test environment
  virtual void SetUp();

  /// @brief Tear-down test environment
  virtual void TearDown();

private:
  std::unique_ptr<core::SingletonManager> singletonManager_;
};

} // namespace unittest

} // namespace sequoia

#endif
