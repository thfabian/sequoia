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

#ifndef SEQUOIA_UNITTEST_FIXTURE_H
#define SEQUOIA_UNITTEST_FIXTURE_H

#include "sequoia/Core/NonCopyable.h"
#include <memory>

namespace sequoia {

namespace unittest {

/// @brief Generic base class of fixtures
///
/// @tparam Fixture   Class to fixture (needs to support `Fixture::SetUp` and `Fixture::TearDown`)
/// @tparam Base      Base fixture class (e.g testing::Test for GTest)
///
/// @ingroup unittest
template <class Fixture, class Base>
class FixtureBase : public Base, public NonCopyable {
  std::unique_ptr<Fixture> fixture_;

protected:
  virtual void SetUp() override {
    fixture_ = std::make_unique<Fixture>();
    fixture_->SetUp();
  }

  virtual void TearDown() override {
    fixture_->TearDown();
    fixture_.release();
  }
};

#define SEQUOIA_TEST_FIXTURE(Name, Base)                                                           \
  class Name : public sequoia::unittest::FixtureBase<Base, testing::Test> {};
#define SEQUOIA_BENCHMARK_FIXTURE(Name, Base)                                                      \
  class Name : public sequoia::unittest::FixtureBase<Base, benchmark::Fixture> {};

} // namespace unittest

} // namespace sequoia

#endif
