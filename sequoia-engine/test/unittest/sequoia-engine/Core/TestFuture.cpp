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

#include "sequoia-engine/Core/Future.h"
#include "sequoia-engine/Core/STLExtras.h"
#include <gtest/gtest.h>
#include <thread>

using namespace sequoia;

namespace {

struct Value {
  int Two = 2;
  float Pi = 3.0f;
};

TEST(FutureTest, DefaultConstructor) {
  Future<int> f1;
  EXPECT_FALSE(f1.valid());

  Future<void> f2;
  EXPECT_FALSE(f2.valid());
}

TEST(FutureTest, CopyConstructor) {
  auto f1 = Future<int>::create(5);
  Future<int> f2(f1);
  EXPECT_EQ(f2.getState().use_count(), 2);
}

TEST(FutureTest, MoveConstructor) {
  auto f1 = Future<int>::create(5);
  Future<int> f2(std::move(f1));
  EXPECT_EQ(f2.getState().use_count(), 1);
}

TEST(FutureTest, ValueInitializedFuture) {
  auto f1 = Future<int>::create(5);
  EXPECT_TRUE(f1.valid());
  EXPECT_EQ(f1.get(), 5);
  EXPECT_FALSE(f1.valid());

  std::shared_ptr<Value> valuePtr = std::make_shared<Value>();
  auto f2 = Future<std::shared_ptr<Value>>::create(valuePtr);

  EXPECT_TRUE(f2.valid());
  EXPECT_EQ(valuePtr.use_count(), 2);

  auto valuePtrGet = f2.get();
  EXPECT_EQ(valuePtrGet->Two, 2);
  EXPECT_EQ(valuePtrGet->Pi, 3.0f);
  EXPECT_FALSE(f2.valid());
}

TEST(FutureTest, Error) {
  auto f1 = Future<int>::create(5);

  std::runtime_error e("Error");
  f1.getState()->setError(std::make_exception_ptr<std::runtime_error>(e));

  EXPECT_TRUE(f1.valid());
  EXPECT_THROW(f1.get(), std::runtime_error);
  EXPECT_FALSE(f1.valid());
}

template <class FunctorType>
auto makeTask(FunctorType&& function) {
  using ReturnType = typename core::function_return_t<decltype(function)>;
  return std::make_shared<FutureTask<ReturnType>>(std::forward<FunctorType>(function));
}

TEST(FutureTest, FutureTask) {
  auto task = makeTask([]() { return int(2); });
  auto future = task->getFuture();

  std::thread t([&task]() { task->run(); });
  int value = future.get();
  t.join();

  EXPECT_EQ(value, 2);
  EXPECT_FALSE(future.valid());
}

TEST(FutureTest, FutureTaskError) {
  auto task = makeTask([]() -> void { throw std::runtime_error("Error"); });
  auto future = task->getFuture();

  std::thread t([&task]() { task->run(); });
  EXPECT_THROW(future.get(), std::runtime_error);
  t.join();
}

} // anonymous namespace
