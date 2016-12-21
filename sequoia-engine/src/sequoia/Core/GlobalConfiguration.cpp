//===-- sequoia/Core/GlobalConfiguration.cpp ----------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//
//
/// @file
/// Global configuration class.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Core/Exception.h"
#include "sequoia/Core/GlobalConfiguration.h"
#include <OGRE/OgreLogManager.h>
#include <boost/property_tree/info_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <iostream>
#include <sstream>

template <>
sequoia::core::GlobalConfiguration*
    Ogre::Singleton<sequoia::core::GlobalConfiguration>::msSingleton = nullptr;

namespace {

template <class StringType>
struct ToString {
  template <class T>
  inline static std::string apply(T&& value) {
    return std::to_string(std::forward<T>(value));
  }
};

template <>
struct ToString<std::wstring> {
  template <class T>
  inline static std::wstring apply(T&& value) {
    return std::to_wstring(std::forward<T>(value));
  }
};
}

namespace sequoia {

namespace core {

GlobalConfiguration::GlobalConfiguration()
    : ptree_(std::make_shared<boost::property_tree::ptree>()) {}

void GlobalConfiguration::put(const std::string& path, const bool& value) {
  put(path, ToString<StringType>::apply(value));
}

void GlobalConfiguration::put(const std::string& path, const int& value) {
  put(path, ToString<StringType>::apply(value));
}

void GlobalConfiguration::put(const std::string& path, const float& value) {
  put(path, ToString<StringType>::apply(value));
}

void GlobalConfiguration::put(const std::string& path, const StringType& value) {
  ptree_->put(path, value);
}

bool GlobalConfiguration::getBoolean(const std::string& path, bool defaultValue) const {
  return ptree_->get<bool>(path, defaultValue);
}

int GlobalConfiguration::getInteger(const std::string& path, int defaultValue) const {
  return ptree_->get<int>(path, defaultValue);
}

float GlobalConfiguration::getFloat(const std::string& path, float defaultValue) const {
  return ptree_->get<float>(path, defaultValue);
}

GlobalConfiguration::StringType GlobalConfiguration::getString(const std::string& path,
                                                               StringType defaultValue) const {
  return ptree_->get<StringType>(path, defaultValue);
}

std::string GlobalConfiguration::toString(ParserKind parser) const {
  std::stringstream ss;

  try {
    switch(parser) {
    case ParserKind::XML:
      boost::property_tree::write_xml(ss, *ptree_);
    case ParserKind::JSON:
      boost::property_tree::write_json(ss, *ptree_);
    case ParserKind::INI:
      boost::property_tree::write_ini(ss, *ptree_);
    case ParserKind::INFO:
      boost::property_tree::write_info(ss, *ptree_);
    }
  } catch(boost::property_tree::ptree_error& e) {
    throw ParserException(e.what());
  }
  return ss.str();
}

std::ostream& operator<<(std::ostream& stream, const GlobalConfiguration& config) {
  return (stream << config.toString());
}

void GlobalConfiguration::addSkipNode(const std::string& node) { skipNodes_.push_back(node); }

} // namespace core

} // namespace sequoia
