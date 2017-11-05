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

#include "sequoia-engine/Core/Options.h"
#include "sequoia-engine/Core/Assert.h"
#include "sequoia-engine/Core/Exception.h"
#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Core/Logging.h"
#include "sequoia-engine/Core/StringSwitch.h"
#include "sequoia-engine/Core/StringUtil.h"
#include "sequoia-engine/Core/Unreachable.h"
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <ostream>
#include <sstream>

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
                      "  CommandLine = {}\n"
                      "  CommandLineShort = {}\n"
                      "  CommandLineHasValue = {}\n"
                      "  CommandLineMetaVar = {}\n"
                      "  DocString = {}\n"
                      "]",
                      CommandLine, CommandLineShort, CommandLineHasValue, CommandLineMetaVar,
                      DocString);
}

Options::Options() {
  // Core
  setBool("Core.Debug", false);
  setMetaData(
      "Core.Debug",
      OptionMetaData{
          "debug", "d", false, "",
          "Enable rigorous error checking and logging, especially of the rendering API calls"});

  // Render
  setString("Render.WindowMode", "window");
  setMetaData("Render.WindowMode",
              OptionMetaData{"window-mode", "", true, "MODE",
                             "Set the window mode to MODE, where MODE is one of "
                             "[window,fullscreen,windowed-fullscreen]"});

  setInt("Render.Monitor", -1);
  setMetaData("Render.Monitor",
              OptionMetaData{"monitor", "", true, "MONITOR", "Set the MONITOR to use, -1 indicates "
                                                             "the primary monitor should be used "
                                                             "which is the default behaviour"});
  setInt("Render.MSAA", 0);
  setBool("Render.VSync", true);
  setInt("Render.GLMajorVersion", 4);
  setInt("Render.GLMinorVersion", 5);
}

OptionMetaData& Options::getMetaData(const std::string& name) {
  auto it = optionsMetaData_.find(name);
  if(it == optionsMetaData_.end())
    SEQUOIA_THROW(core::Exception, "meta data of option '{}' does not exist", name);
  return it->second;
}

void Options::write(const std::string& file) const {
  Log::info("Writing Options to \"{}\" ...", file);

  boost::property_tree::ptree ptree;
  for(const auto& option : options_)
    ptree.put(option.first, option.second);

  try {
    boost::property_tree::write_ini(file, ptree);
  } catch(boost::property_tree::ini_parser_error& e) {
    Log::warn("Failed to write Options : {}", e.what());
    SEQUOIA_THROW(core::Exception, "failed to write Options : {}", e.what());
  }

  Log::info("Successfully wrote Options to \"{}\"", file);
}

void Options::read(const std::string& file) {
  Log::info("Reading Options from \"{}\" ...", file);

  boost::property_tree::ptree ptree;
  try {
    boost::property_tree::read_ini(file, ptree);
  } catch(boost::property_tree::ini_parser_error& e) {
    Log::warn("Failed to read Options : {}", e.what());
    SEQUOIA_THROW(core::Exception, "failed to read Options : {}", e.what());
  }

  for(const auto& base : ptree) {
    std::string baseStr = base.first + ".";
    for(const auto& option : base.second) {
      if(option.second.size() != 0)
        SEQUOIA_THROW(core::Exception, "invalid ini file");
      options_[baseStr + option.first] = option.second.get_value<std::string>();
    }
  }

  Log::info("Successfully read Options to \"{}\"", file);
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

void Options::setImpl(const std::string& name, bool value) noexcept {
  SEQUOIA_ASSERT_MSG(std::count(name.begin(), name.end(), '.') == 1,
                     "Option is not of the form \"X.Y\" i.e contains more (or less) than one dot");
  options_[name] = std::to_string(value);
}

void Options::setImpl(const std::string& name, int value) noexcept {
  SEQUOIA_ASSERT_MSG(std::count(name.begin(), name.end(), '.') == 1,
                     "Option is not of the form \"X.Y\" i.e contains more (or less) than one dot");
  options_[name] = std::to_string(value);
}

void Options::setImpl(const std::string& name, float value) noexcept {
  SEQUOIA_ASSERT_MSG(std::count(name.begin(), name.end(), '.') == 1,
                     "Option is not of the form \"X.Y\" i.e contains more (or less) than one dot");
  options_[name] = std::to_string(value);
}

void Options::setImpl(const std::string& name, std::string value) noexcept {
  SEQUOIA_ASSERT_MSG(std::count(name.begin(), name.end(), '.') == 1,
                     "Option is not of the form \"X.Y\" i.e contains more (or less) than one dot");
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

} // namespace core

} // namespace sequoia
