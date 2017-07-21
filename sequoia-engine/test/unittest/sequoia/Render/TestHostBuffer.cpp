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

#include "sequoia/Render/HostBuffer.h"
#include <gtest/gtest.h>

using namespace sequoia::render;

namespace {

TEST(HostBufferTest, Allocate) {
  auto buffer = HostBuffer::create(1024);

  EXPECT_EQ(buffer->getNumBytes(), 1024);
  EXPECT_NE(buffer->getData(), nullptr);

  EXPECT_FALSE(buffer->hasShadowBuffer());
  EXPECT_TRUE(buffer->isSystemRAM());

  // Lock & Unlock
  buffer->lock(Buffer::LO_Normal);
  EXPECT_EQ(buffer->getData(), buffer->get());
  buffer->unlock();
}

TEST(HostBufferTest, ReadAndWrite) {
  std::vector<double> vec = {2.0, 3.0, 4.0};

  auto buffer = HostBuffer::create(3 * sizeof(double));
  buffer->write(vec.data(), 0, vec.size() * sizeof(double), true);

  EXPECT_FLOAT_EQ(buffer->getDataAs<double>()[0], 2.0);
  EXPECT_FLOAT_EQ(buffer->getDataAs<double>()[1], 3.0);
  EXPECT_FLOAT_EQ(buffer->getDataAs<double>()[2], 4.0);

  std::vector<double> vecRef(3);
  buffer->read(0, vecRef.size() * sizeof(double), vecRef.data());
  EXPECT_EQ(vec, vecRef);
}

TEST(HostBufferTest, ReadAndWriteOffset) {
  std::vector<double> vec = {2.0, 3.0, 4.0};

  auto buffer = HostBuffer::create(3 * sizeof(double));
  buffer->write(vec.data(), 0, vec.size() * sizeof(double), true);

  std::vector<double> vecRef(2);
  buffer->read(1 * sizeof(double), vecRef.size() * sizeof(double), vecRef.data());

  EXPECT_FLOAT_EQ(vecRef[0], vec[1]);
  EXPECT_FLOAT_EQ(vecRef[1], vec[2]);
}

} // anonymous namespace
