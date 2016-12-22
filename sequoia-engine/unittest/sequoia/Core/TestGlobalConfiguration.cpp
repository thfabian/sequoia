//===--------------------------------------------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Core/GlobalConfiguration.h"
#include "sequoia/Core/Exception.h"
#include <boost/property_tree/ptree.hpp>
#include <gtest/gtest.h>

using namespace sequoia::core;

namespace {

TEST(GlobalConfigurationTest, Construction) {
  auto config = std::make_unique<GlobalConfiguration>();
  ASSERT_NE(config->ptree(), nullptr);
}

TEST(GlobalConfigurationTest, PutAndGet) {
  auto config = std::make_unique<GlobalConfiguration>();

  config->put("Test.Boolean", true);
  config->put("Test.Float", 2.0f);
  config->put("Test.Integer", 3);
  config->put("Test.String", "four");
  config->put("Test.Path", boost::filesystem::path("/five/six/seven"));

  // Existing
  EXPECT_EQ(config->getBoolean("Test.Boolean", false), true);
  EXPECT_EQ(config->getFloat("Test.Float", -2.0f), 2.0f);
  EXPECT_EQ(config->getInteger("Test.Integer", -3), 3);
  EXPECT_EQ(config->getString("Test.String", "minus_four"), "four");
  EXPECT_EQ(config->getPath("Test.Path", boost::filesystem::path("/minus_five")),
            boost::filesystem::path("/five/six/seven"));

  // Non-existant
  EXPECT_EQ(config->getBoolean("Test.BooleanX", false), false);
}

TEST(GlobalConfigurationTest, PutArrayAndGetArray) {
  auto config = std::make_unique<GlobalConfiguration>();
  using path = boost::filesystem::path;

  config->put("Test.IntegerVec", std::vector<int>{0, 1, 2});
  config->put("Test.FloatVec", std::vector<float>{0.1f, 1.2f, 2.3f});
  config->put("Test.StringVec", std::vector<std::string>{"zero", "one", "two"});
  config->put("Test.PathVec", std::vector<path>{path("/a"), path("/a/a")});

  EXPECT_EQ(config->getIntegerVector("Test.IntegerVec"), (std::vector<int>{0, 1, 2}));
  EXPECT_TRUE(config->getIntegerVector("Test.IntegerVecX").empty());
  
  EXPECT_EQ(config->getFloatVector("Test.FloatVec"), (std::vector<float>{0.1f, 1.2f, 2.3f}));
  EXPECT_EQ(config->getStringVector("Test.StringVec"),
            (std::vector<std::string>{"zero", "one", "two"}));
  EXPECT_EQ(config->getPathVector("Test.PathVec"), (std::vector<path>{path("/a"), path("/a/a")}));

  // Errors
  EXPECT_THROW(config->getIntegerVector("Test.StringVec"), ParserException);
}

TEST(GlobalConfigurationTest, SaveAndLoad) {
  {
    auto config = std::make_unique<GlobalConfiguration>();

    config->put("Test.Boolean", true);
    config->put("Test.Float", 2.0f);
    config->put("Test.Integer", 3);
    config->put("Test.String", "four");
    config->put("Test.Path", boost::filesystem::path("/five/six/seven"));
    
    config->put("SkipNode.Test", true);
    config->addSkipNode("SkipNode");
    
    config->save("TestConfig.xml");
    config->save("TestConfig.ini");
    config->save("TestConfig.json");
    config->save("TestConfig.info");
  }

  {
    auto config = std::make_unique<GlobalConfiguration>();
    config->load("TestConfig.xml");

    EXPECT_EQ(config->getBoolean("Test.Boolean", false), true);
    EXPECT_EQ(config->getFloat("Test.Float", -2.0f), 2.0f);
    EXPECT_EQ(config->getInteger("Test.Integer", -3), 3);
    EXPECT_EQ(config->getString("Test.String", "minus_four"), "four");
    EXPECT_EQ(config->getPath("Test.Path", boost::filesystem::path("/minus_five")),
              boost::filesystem::path("/five/six/seven"));

    EXPECT_EQ(config->getBoolean("SkipNode.Test", false), false);    
    
    config->clear();
    config->load("TestConfig.json");

    EXPECT_EQ(config->getBoolean("Test.Boolean", false), true);
    EXPECT_EQ(config->getFloat("Test.Float", -2.0f), 2.0f);
    EXPECT_EQ(config->getInteger("Test.Integer", -3), 3);
    EXPECT_EQ(config->getString("Test.String", "minus_four"), "four");
    EXPECT_EQ(config->getPath("Test.Path", boost::filesystem::path("/minus_five")),
              boost::filesystem::path("/five/six/seven"));

    config->clear();
    config->load("TestConfig.ini");
    EXPECT_EQ(config->getBoolean("Test.Boolean", false), true);
    EXPECT_EQ(config->getFloat("Test.Float", -2.0f), 2.0f);
    EXPECT_EQ(config->getInteger("Test.Integer", -3), 3);
    EXPECT_EQ(config->getString("Test.String", "minus_four"), "four");
    EXPECT_EQ(config->getPath("Test.Path", boost::filesystem::path("/minus_five")),
              boost::filesystem::path("/five/six/seven"));

    config->clear();
    config->load("TestConfig.info");
    EXPECT_EQ(config->getBoolean("Test.Boolean", false), true);
    EXPECT_EQ(config->getFloat("Test.Float", -2.0f), 2.0f);
    EXPECT_EQ(config->getInteger("Test.Integer", -3), 3);
    EXPECT_EQ(config->getString("Test.String", "minus_four"), "four");
    EXPECT_EQ(config->getPath("Test.Path", boost::filesystem::path("/minus_five")),
              boost::filesystem::path("/five/six/seven"));
  }
}

} // anonymous namespace
