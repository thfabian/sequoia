//===--------------------------------------------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Core/Version.h"
#include <gtest/gtest.h>

using namespace sequoia::core;

namespace {

TEST(Version, Construction) {
  Version version(1, 60, 0);
  EXPECT_EQ(version.major(), 1);
  EXPECT_EQ(version.minor(), 60);
  EXPECT_EQ(version.patch(), 0);

  Version currentVersion = Version::currentVersion();
  EXPECT_EQ(currentVersion.major(), SEQUOIA_VERSION_MAJOR);
  EXPECT_EQ(currentVersion.minor(), SEQUOIA_VERSION_MINOR);
  EXPECT_EQ(currentVersion.patch(), SEQUOIA_VERSION_PATCH);
}

TEST(Version, Coversion) {
  Version versionTripple(1, 60, 2);
  EXPECT_EQ(Version::toSingle(versionTripple), 106002);

  Version fromSingle = Version::fromSingle(106002);
  EXPECT_EQ(fromSingle.major(), 1);
  EXPECT_EQ(fromSingle.minor(), 60);
  EXPECT_EQ(fromSingle.patch(), 2);
}

TEST(Version, Comparison) {
  Version versionA(1, 60, 1);
  Version versionB(1, 60, 2);

  EXPECT_TRUE(versionA == versionA);
  EXPECT_TRUE(versionA != versionB);
  EXPECT_TRUE(versionA <= versionB);
  EXPECT_TRUE(versionB >= versionA);
}

TEST(Version, ToString) {
  std::stringstream ss;
  ss << Version(1, 60, 2);
  EXPECT_STREQ(ss.str().c_str(), "1.60.2");
}

} // anonymous namespace
