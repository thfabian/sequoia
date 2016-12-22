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
#include "sequoia/Core/Format.h"
#include "sequoia/Core/GlobalConfiguration.h"
#include "sequoia/Core/String.h"
#include "sequoia/Core/StringRef.h"
#include <boost/property_tree/info_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

template <>
sequoia::core::GlobalConfiguration*
    Ogre::Singleton<sequoia::core::GlobalConfiguration>::msSingleton = nullptr;

namespace {

template <class StringType>
struct StringUtil {
  template <class T>
  inline static std::string toString(T&& value) {
    return std::to_string(std::forward<T>(value));
  }

  inline static int toInteger(const std::string& value) { return std::stoi(value); }
  inline static float toFloat(const std::string& value) { return std::stof(value); }
};

template <>
struct StringUtil<std::wstring> {
  template <class T>
  inline static std::wstring toString(T&& value) {
    return std::to_wstring(std::forward<T>(value));
  }

  inline static int toInteger(const std::wstring& value) { return std::stoi(value); }
  inline static float toFloat(const std::wstring& value) { return std::stof(value); }
};
}

namespace sequoia {

namespace core {

GlobalConfiguration::GlobalConfiguration() : ptree_(std::make_shared<TreeType>()) {}

void GlobalConfiguration::put(const std::string& path, const bool& value) {
  ptree_->put(path, StringUtil<TreeType::data_type>::toString(value));
}

void GlobalConfiguration::put(const std::string& path, const int& value) {
  ptree_->put(path, StringUtil<TreeType::data_type>::toString(value));
}

void GlobalConfiguration::put(const std::string& path, const float& value) {
  ptree_->put(path, StringUtil<TreeType::data_type>::toString(value));
}

void GlobalConfiguration::put(const std::string& path, const char* value) {
  put(path, std::string(value));
}

void GlobalConfiguration::put(const std::string& path, const std::string& value) {
#ifdef SEQUOIA_ON_WIN32
  ptree_->put(path, String(value).asWStr());
#else
  ptree_->put(path, value);
#endif
}

void GlobalConfiguration::put(const std::string& path, const boost::filesystem::path& value) {
  ptree_->put(path, value.string());
}

void GlobalConfiguration::put(std::string path, ArrayRef<int> value) {
  path += ".value";
  for(const auto& v : value)
    ptree_->add(path, StringUtil<TreeType::data_type>::toString(v));
}

void GlobalConfiguration::put(std::string path, ArrayRef<float> value) {
  path += ".value";
  for(const auto& v : value)
    ptree_->add(path, StringUtil<TreeType::data_type>::toString(v));
}

void GlobalConfiguration::put(std::string path, ArrayRef<std::string> value) {
  path += ".value";
  for(const auto& v : value) {
#ifdef SEQUOIA_ON_WIN32
    ptree_->add(path, String(v).asWStr());
#else
    ptree_->add(path, v);
#endif
  }
}

void GlobalConfiguration::put(std::string path, ArrayRef<boost::filesystem::path> value) {
  path += ".value";
  for(const auto& v : value)
    ptree_->add(path, v.string());
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

std::string GlobalConfiguration::getString(const std::string& path,
                                           std::string defaultValue) const {
#ifdef SEQUOIA_ON_WIN32
  return String(ptree_->get<std::wstring>(path, String(defaultValue).asWStr())).asUTF8();
#else
  return ptree_->get<std::string>(path, defaultValue);
#endif
}

std::vector<int> GlobalConfiguration::getIntegerVector(const std::string& path) const {
  std::vector<int> vec;

  try {
    for(auto it : ptree_->get_child(path))
      vec.push_back(StringUtil<TreeType::data_type>::toInteger(it.second.data()));
  } catch(boost::property_tree::ptree_bad_path& e) {
  } catch(std::logic_error& e) {
    SEQUOIA_THROW(ParserException, e.what());
  }

  return vec;
}

std::vector<float> GlobalConfiguration::getFloatVector(const std::string& path) const {
  std::vector<float> vec;

  try {
    for(auto it : ptree_->get_child(path))
      vec.push_back(StringUtil<TreeType::data_type>::toFloat(it.second.data()));
  } catch(boost::property_tree::ptree_bad_path& e) {
  } catch(std::logic_error& e) {
    SEQUOIA_THROW(ParserException, e.what());
  }

  return vec;
}

std::vector<std::string> GlobalConfiguration::getStringVector(const std::string& path) const {
  std::vector<std::string> vec;

  try {
    for(auto it : ptree_->get_child(path))
#ifdef SEQUOIA_ON_WIN32
      vec.push_back(String(it.second.data()).asUTF8());
#else
      vec.push_back(it.second.data());
#endif
  } catch(boost::property_tree::ptree_bad_path& e) {
  }

  return vec;
}

std::vector<boost::filesystem::path>
GlobalConfiguration::getPathVector(const std::string& path) const {
  std::vector<boost::filesystem::path> vec;

  try {
    for(auto it : ptree_->get_child(path))
      vec.push_back(it.second.data());
  } catch(boost::property_tree::ptree_bad_path& e) {
  }

  return vec;
}

boost::filesystem::path GlobalConfiguration::getPath(const std::string& path,
                                                     boost::filesystem::path defaultValue) const {
  return ptree_->get<boost::filesystem::path::string_type>(path, defaultValue.string());
}

void GlobalConfiguration::clear() noexcept { ptree_->clear(); }

static void toStream(std::ostream& stream, GlobalConfiguration::TreeType& tree,
                     GlobalConfiguration::ParserKind parser) {
  try {
    switch(parser) {
    case GlobalConfiguration::ParserKind::XML:
      boost::property_tree::write_xml(stream, tree);
      break;
    case GlobalConfiguration::ParserKind::JSON:
      boost::property_tree::write_json(stream, tree);
      break;
    case GlobalConfiguration::ParserKind::INI:
      boost::property_tree::write_ini(stream, tree);
      break;
    case GlobalConfiguration::ParserKind::INFO:
      boost::property_tree::write_info(stream, tree);
      break;
    }
  } catch(boost::property_tree::ptree_error& e) {
    SEQUOIA_THROW(ParserException, e.what());
  }
}

static void fromStream(std::istream& stream, GlobalConfiguration::TreeType& tree,
                       GlobalConfiguration::ParserKind parser) {
  try {
    switch(parser) {
    case GlobalConfiguration::ParserKind::XML:
      boost::property_tree::read_xml(stream, tree);
      break;
    case GlobalConfiguration::ParserKind::JSON:
      boost::property_tree::read_json(stream, tree);
      break;
    case GlobalConfiguration::ParserKind::INI:
      boost::property_tree::read_ini(stream, tree);
      break;
    case GlobalConfiguration::ParserKind::INFO:
      boost::property_tree::read_info(stream, tree);
      break;
    }
  } catch(boost::property_tree::ptree_error& e) {
    SEQUOIA_THROW(ParserException, e.what());
  }
}

static GlobalConfiguration::ParserKind fileExtensionToParser(boost::filesystem::path& path) {
#ifdef SEQUOIA_ON_WIN32
  std::string extStr = String(path.extension().string()).asUTF8();
#else
  std::string extStr = path.extension().string();
#endif
  StringRef ext(extStr);

  if(ext.lower() == ".xml")
    return GlobalConfiguration::ParserKind::XML;
  else if(ext.lower() == ".json")
    return GlobalConfiguration::ParserKind::JSON;
  else if(ext.lower() == ".ini")
    return GlobalConfiguration::ParserKind::INI;
  else if(ext.lower() == ".info")
    return GlobalConfiguration::ParserKind::INFO;
  throw IOException(format("cannot deduce parser from file extension: \"%s\"", extStr));
}

std::string GlobalConfiguration::toString(ParserKind parser) const {
  std::stringstream ss;
  toStream(ss, *ptree_, parser);
  return ss.str();
}

void GlobalConfiguration::save(boost::filesystem::path path) const {
  std::ofstream ofs(path.string());
  if(!ofs.is_open())
    SEQUOIA_THROW(IOException, format(CSTR("cannot open file: %s"), path));
    
  if(!skipNodes_.empty()) {
    auto ptree = *ptree_;

    for(const auto skip : skipNodes_)
      ptree.erase(skip);

    toStream(ofs, ptree, fileExtensionToParser(path));
  } else
    toStream(ofs, *ptree_, fileExtensionToParser(path));
}

void GlobalConfiguration::load(boost::filesystem::path path) {
  std::ifstream ifs(path.string());
  if(!ifs.is_open())
    SEQUOIA_THROW(IOException, format(CSTR("cannot open file: %s"), path));
  fromStream(ifs, *ptree_, fileExtensionToParser(path));
}

std::ostream& operator<<(std::ostream& stream, const GlobalConfiguration& config) {
  return (stream << config.toString());
}

std::ostream& operator<<(std::ostream& stream, const GlobalConfiguration* config) {
  return (stream << *config);
}

void GlobalConfiguration::addSkipNode(const std::string& node) { skipNodes_.push_back(node); }

} // namespace core

} // namespace sequoia
