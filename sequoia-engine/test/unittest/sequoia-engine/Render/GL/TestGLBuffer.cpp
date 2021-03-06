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

#include "sequoia-engine/Core/StringUtil.h"
#include "sequoia-engine/Render/GL/GLBuffer.h"
#include "sequoia-engine/Unittest/RenderSetup.h"
#include <algorithm>
#include <gtest/gtest.h>
#include <iostream>
#include <limits>
#include <random>
#include <vector>

using namespace sequoia;
using namespace sequoia::unittest;
using namespace sequoia::render;

namespace {

SEQUOIA_TESTCASEFIXTURE(GLBufferTest, RenderSetup);

/// @brief Generate a random vector of type `T`
template <class T>
std::vector<T> makeRandomVector(std::size_t size) {
  std::uniform_int_distribution<T> distribution(std::numeric_limits<T>::min(),
                                                std::numeric_limits<T>::max());
  std::default_random_engine generator;

  std::vector<T> vec(size);
  std::generate(vec.begin(), vec.end(), [&]() { return distribution(generator); });
  return vec;
}

/// @brief Compare vector `vec` to `vecRef`
template <class T>
void compareVector(const std::vector<T>& vec, const std::vector<T>& vecRef, const char* file,
                   int line) {
  ASSERT_EQ(vec.size(), vecRef.size()) << "comparison at " << file << ":" << line;
  for(std::size_t i = 0; i < vecRef.size(); ++i)
    ASSERT_EQ(vec[i], vecRef[i]) << "with i = " << i << " comparison at " << file << ":" << line;
}
#define ASSERT_VECTOR_EQ(vec, vecRef) compareVector(vec, vecRef, __FILE__, __LINE__)

/// @brief Get number of stored bytes in `vec`
template <class T>
std::size_t getNumBytes(const std::vector<T>& vec) {
  return vec.size() * sizeof(T);
}

TEST_F(GLBufferTest, ReadAndWrite) {
  auto vec = makeRandomVector<int>(8);
  auto vecRef = decltype(vec)(vec.size(), 0);

  GLBuffer buffer(GL_ARRAY_BUFFER);
  buffer.allocate(getNumBytes(vec), Buffer::UH_Dynamic);
  buffer.write(vec.data(), 0, getNumBytes(vec));
  buffer.read(0, getNumBytes(vecRef), vecRef.data());

  ASSERT_VECTOR_EQ(vec, vecRef);
}

TEST_F(GLBufferTest, ReadAndWriteOffset) {
  auto vec = makeRandomVector<int>(8);
  auto vecRef = decltype(vec)(vec.size(), 0);

  GLBuffer buffer(GL_ARRAY_BUFFER);
  buffer.allocate(getNumBytes(vec), Buffer::UH_Dynamic);

  std::size_t offset = 4 * sizeof(int);
  buffer.write(vec.data(), offset, getNumBytes(vec) - offset);
  buffer.read(offset, getNumBytes(vecRef) - offset, vecRef.data());

  // Copy the rest
  std::copy(vec.begin() + 4, vec.end(), vecRef.begin() + 4);

  ASSERT_VECTOR_EQ(vec, vecRef);
}

TEST_F(GLBufferTest, LockAndUnlock) {
  GLBuffer buffer(GL_ARRAY_BUFFER);
  buffer.allocate(64, Buffer::UH_StaticWriteOnly);

  void* src = buffer.lock(Buffer::LO_WriteOnly);
  EXPECT_NE(src, nullptr);
  buffer.unlock();
}

#undef ASSERT_VECTOR_EQ

} // anonymous namespace
