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

/// @brief Generic base class for test fixtures
/// @ingroup unittest
template <class FixtureT>
class TestFixture : public testing::Test, public NonCopyable {
  std::unique_ptr<FixtureT> fixture_;

public:
  virtual void SetUp() override {
    fixture_ = std::make_unique<FixtureT>();
    fixture_->SetUp();
  }

  virtual void TearDown() override {
    fixture_->TearDown();
    fixture_.release();
  }
};

/// @brief Generic base class for benchmark fixtures
/// @ingroup unittest
template <class FixtureT>
class BenchmarkFixture : public benchmark::Fixture, public NonCopyable {
  std::unique_ptr<FixtureT> fixture_;

public:
  virtual void SetUp(benchmark::State& st) override {
    (void) st;
    fixture_ = std::make_unique<FixtureT>();
    fixture_->SetUp();
  }

  virtual void TearDown(benchmark::State& st) override {
    (void) st;
    fixture_->TearDown();
    fixture_.release();
  }
};

} // namespace unittest

} // namespace sequoia

#endif
