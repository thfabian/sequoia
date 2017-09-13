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

/// @brief Sets up the stuff for *each* test in the test case.
///
/// @tparam FixtureT  Type of test fixture
///
/// This runs `FixtureT::SetUp` before *each* test and `FixtureT::TearDown` after the test has run.
///
/// @ingroup unittest
template <class FixtureT>
class TestFixture : public testing::Test, public NonCopyable {
  std::unique_ptr<FixtureT> fixture_ = nullptr;

public:
  /// @brief Run before *each* test
  virtual void SetUp() override {
    fixture_ = std::make_unique<FixtureT>();
    fixture_->SetUp();
  }

  /// @brief Run after *each* test
  virtual void TearDown() override {
    fixture_->TearDown();
    fixture_.release();
  }
};

#define SEQUOIA_DEFINE_TESTCASEFIXTURE_IMPL(TestCaseName, FixtureT)                                \
  class TestCaseName : public ::testing::Test, public ::sequoia::core::NonCopyable {               \
  public:                                                                                          \
    static std::unique_ptr<FixtureT> Fixture;                                                      \
    static void SetUpTestCase() {                                                                  \
      Fixture = std::make_unique<FixtureT>();                                                      \
      Fixture->SetUp();                                                                            \
    }                                                                                              \
    static void TearDownTestCase() {                                                               \
      Fixture->TearDown();                                                                         \
      Fixture.release();                                                                           \
    }                                                                                              \
  };

/// @brief Sets up the stuff shared by *all* tests in a test case
///
/// @param TestCaseName   Name of the test case i.e test fixture class
/// @param FixtureT       Type of test fixture
///
/// This runs `FixtureT::SetUp` before the test-case (i.e before all tests) and `FixtureT::TearDown`
/// after all tests have run.
///
/// @see sequoia::unittest::TestCaseFixture
/// @ingroup unittest
#define SEQUOIA_TESTCASEFIXTURE(TestCaseName, FixtureT)                                            \
  SEQUOIA_DEFINE_TESTCASEFIXTURE_IMPL(TestCaseName, FixtureT)                                      \
  std::unique_ptr<FixtureT> TestCaseName::Fixture = nullptr

/// @brief Sets up the stuff shared by *all* tests in a test case
///
/// This version allows typed tests (`TYPED_TEST_CASE`).
///
/// @see SEQUOIA_SETUP_TESTCASEFIXTURE
/// @ingroup unittest
#define SEQUOIA_TESTCASEFIXTURE_TEMPLATE(TestCaseName, FixtureT)                                   \
  template <class T>                                                                               \
  SEQUOIA_DEFINE_TESTCASEFIXTURE_IMPL(TestCaseName, FixtureT)                                      \
  template <class T>                                                                               \
  std::unique_ptr<FixtureT> TestCaseName<T>::Fixture = nullptr

/// @brief Generic base class for benchmark fixtures
/// @ingroup unittest
template <class FixtureT>
class BenchmarkFixture : public benchmark::Fixture, public NonCopyable {
  std::unique_ptr<FixtureT> fixture_;

public:
  virtual void SetUp(benchmark::State& st) override {
    (void)st;
    fixture_ = std::make_unique<FixtureT>();
    fixture_->SetUp();
  }

  virtual void TearDown(benchmark::State& st) override {
    (void)st;
    fixture_->TearDown();
    fixture_.release();
  }
};

} // namespace unittest

} // namespace sequoia

#endif
