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

#include "sequoia-engine/Core/Assert.h"
#include "sequoia-engine/Core/Exception.h"
#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Core/Logging.h"
#include "sequoia-engine/Core/Options.h"
#include "sequoia-engine/Core/StringSwitch.h"
#include "sequoia-engine/Core/StringUtil.h"
#include "sequoia-engine/Core/Unreachable.h"
#include "sequoia-engine/Core/Xml.h"
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <fstream>
#include <sstream>

#include <iostream>

namespace sequoia {

namespace core {

namespace {

template <class... Args>
bool isOneOf(const std::string& str, Args&&... args) {
  for(const auto& arg : {args...})
    if(arg == str)
      return true;
  return false;
}

template <class T>
struct InvalidType {
  using type = void;
};

template <class T>
T convert(const std::string& key, const std::string& value) {
  static_assert(std::is_same<typename InvalidType<T>::type, void>::value, "invalid type");
}

template <>
bool convert<bool>(const std::string& key, const std::string& value) {
  try {
    return static_cast<bool>(boost::lexical_cast<float>(value));
  } catch(const boost::bad_lexical_cast&) {
  }

  if(isOneOf(value, "true", "True", "TRUE"))
    return true;
  else if(isOneOf(value, "false", "False", "FALSE"))
    return false;

  SEQUOIA_THROW(core::Exception, "cannot convert '{}' (\"{}\") to 'bool'", key, value);
  return bool{};
}

template <>
int convert<int>(const std::string& key, const std::string& value) {
  try {
    return static_cast<int>(boost::lexical_cast<float>(value));
  } catch(const boost::bad_lexical_cast& e) {
    SEQUOIA_THROW(core::Exception, "cannot convert '{}' (\"{}\") to 'int' : {}", key, value,
                  e.what());
  }
  return int{}; // unreachable
}

template <>
float convert<float>(const std::string& key, const std::string& value) {
  try {
    return boost::lexical_cast<float>(value);
  } catch(const boost::bad_lexical_cast& e) {
    SEQUOIA_THROW(core::Exception, "cannot convert '{}' (\"{}\") to 'float' : {}", key, value,
                  e.what());
  }
  return float{}; // unreachable
}

template <>
std::string convert<std::string>(const std::string& key, const std::string& value) {
  return value;
}

} // anonymous namespace

std::string OptionMetaData::toString() const {
  return core::format("OptionMetaData[\n"
                      "  CommandLine = \"{}\"\n"
                      "  CommandLineShort = \"{}\"\n"
                      "  CommandLineHasValue = {}\n"
                      "  CommandLineMetaVar = \"{}\"\n"
                      "  DocString = \"{}\"\n"
                      "]",
                      CommandLine, CommandLineShort, CommandLineHasValue, CommandLineMetaVar,
                      DocString);
}

Options::Options() {}

OptionMetaData& Options::getMetaData(const std::string& name) {
  auto it = optionsMetaData_.find(name);
  if(it == optionsMetaData_.end())
    SEQUOIA_THROW(core::Exception, "meta data of option '{}' does not exist", name);
  return it->second;
}

static void writeXML(const boost::property_tree::ptree& children, xml_node node) {
  if(children.empty())
    xml_set_value(node, children.get_value<std::string>());
  else
    for(auto it = children.begin(), end = children.end(); it != end; ++it)
      writeXML(it->second, node.append_child(it->first.c_str()));
}

void Options::write(const std::string& file) const {
  Log::info("Writing Options to \"{}\" ...", file);

  boost::property_tree::ptree ptree;
  for(const auto& option : options_)
    ptree.put(option.first, option.second);

  // The XML writer of property_tree is broken..
  xml_document doc;
  writeXML(ptree, doc.append_child("Config"));

  std::ofstream fout(file);
  if(!fout.is_open())
    SEQUOIA_THROW(core::Exception, "failed to open file: {}", file);

  doc.save(fout, xml_indent());
  Log::info("Successfully wrote Options to \"{}\"", file);
}

static void parseXML(std::string key, xml_node node,
                     std::unordered_map<std::string, std::string>& options) {
  if(!node.text().empty()) {
    options[key] = node.text().as_string();
  } else {
    for(auto it = node.begin(), end = node.end(); it != end; ++it) {
      xml_node child = *it;
      std::string newKey = (key.empty() ? "" : key + ".") + child.name();
      parseXML(std::move(newKey), child, options);
    }
  }
}

bool Options::read(const std::string& file) {
  Log::info("Reading Options from \"{}\" ...", file);

  xml_document doc;
  doc.load_file(file.c_str());
  parseXML("", doc.child("Config"), options_);

  Log::info("Successfully read Options to \"{}\"", file);
  return true;
}

std::string Options::toString() const {
  // Get a sorted list of the options
  std::vector<std::pair<std::string, std::string>> optionVec;
  std::copy(options_.begin(), options_.end(), std::back_inserter(optionVec));
  std::sort(optionVec.begin(), optionVec.end(),
            [](const auto& a, const auto& b) { return a.first < b.first; });

  return core::format(
      "Options[\n"
      "  options = {},\n"
      "  optionsMetaData = {}\n"
      "]",
      core::indent(core::toStringRange(optionVec,
                                       [](const auto& optionPair) {
                                         return core::format("{} = {}", optionPair.first,
                                                             optionPair.second);
                                       })),
      optionsMetaData_.empty()
          ? "null"
          : core::indent(core::toStringRange(optionsMetaData_, [](const auto& optionMetaDataPair) {
              return core::format("{} = {}", optionMetaDataPair.first,
                                  core::indent(optionMetaDataPair.second.toString()));
            })));
}

void Options::setImpl(const std::string& name, bool value, bool isDefault) noexcept {
  if(isDefault)
    options_.emplace(name, std::to_string(value));
  else
    options_[name] = std::to_string(value);
}

void Options::setImpl(const std::string& name, int value, bool isDefault) noexcept {
  if(isDefault)
    options_.emplace(name, std::to_string(value));
  else
    options_[name] = std::to_string(value);
}

void Options::setImpl(const std::string& name, float value, bool isDefault) noexcept {
  if(isDefault)
    options_.emplace(name, std::to_string(value));
  else
    options_[name] = std::to_string(value);
}

void Options::setImpl(const std::string& name, std::string value, bool isDefault) noexcept {
  if(isDefault)
    options_.emplace(name, std::move(value));
  else
    options_[name] = std::move(value);
}

const std::string& Options::getValueImpl(const std::string& name) const {
  auto it = options_.find(name);
  if(it == options_.end())
    SEQUOIA_THROW(core::Exception, "option '{}' does not exist", name);
  return it->second;
}

bool Options::getImpl(const std::string& name, const bool*) const {
  return convert<bool>(name, getValueImpl(name));
}

int Options::getImpl(const std::string& name, const int*) const {
  return convert<int>(name, getValueImpl(name));
}

float Options::getImpl(const std::string& name, const float*) const {
  return convert<float>(name, getValueImpl(name));
}

std::string Options::getImpl(const std::string& name, const std::string*) const {
  return convert<std::string>(name, getValueImpl(name));
}

void setDefaultOptions(const std::shared_ptr<Options>& options) {
  options->setDefaultBool(
      "Core.Debug", false,
      OptionMetaData{
          "debug", "d", false, "",
          "Enable rigorous error checking and logging, especially of the rendering API calls"});
}

} // namespace core

} // namespace sequoia
