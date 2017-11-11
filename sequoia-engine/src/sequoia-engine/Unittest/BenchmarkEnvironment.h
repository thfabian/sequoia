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

#ifndef SEQUOIA_ENGINE_UNITTEST_BENCHMARKENVIRONMENT_H
#define SEQUOIA_ENGINE_UNITTEST_BENCHMARKENVIRONMENT_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/Logging.h"
#include "sequoia-engine/Core/Options.h"
#include "sequoia-engine/Core/Singleton.h"
#include <memory>
#include <stack>

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

  /// @brief Get a **clone** of the current options
  std::shared_ptr<Options> getOptions();

private:
  /// Benchmark logger
  std::unique_ptr<core::Logger> logger_;
  
  /// Options
  std::shared_ptr<Options> options_;
};

} // namespace unittest

} // namespace sequoia

#endif
