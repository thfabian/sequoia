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
#include <benchmark/benchmark.h>
#include <gtest/gtest.h>
#include <memory>

namespace sequoia {

namespace unittest {

/// @brief Generic base class of all test fixtures
/// @ingroup unittest
template <class FixtureT>
class TestFixtureBase : public testing::Test, public NonCopyable {
  std::unique_ptr<FixtureT> fixture_;

protected:
  virtual void SetUp() override {
    fixture_ = std::make_unique<FixtureT>();
    fixture_->SetUp();
  }

  virtual void TearDown() override {
    fixture_->TearDown();
    fixture_.release();
  }
};

/// @brief Generic base class of all benchmark fixtures
/// @ingroup unittest
template <class FixtureT>
class BenchmarkFixtureBase : public benchmark::Fixture, public NonCopyable {
  std::unique_ptr<FixtureT> fixture_;

protected:
  virtual void SetUp(benchmark::State& st) override {
    fixture_ = std::make_unique<FixtureT>();
    fixture_->SetUp();
  }

  virtual void TearDown(benchmark::State& st) override {
    fixture_->TearDown();
    fixture_.release();
  }
};

#define SEQUOIA_TEST_FIXTURE(Name, SetupClass)                                                     \
  class Name : public sequoia::unittest::TestFixtureBase<SetupClass> {};
#define SEQUOIA_BENCHMARK_FIXTURE(Name, SetupClass)                                                \
  class Name : public sequoia::unittest::BenchmarkFixtureBase<SetupClass> {};

} // namespace unittest

} // namespace sequoia

#endif
