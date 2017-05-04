//===--------------------------------------------------------------------------------*- C++ -*-===//
//                         _____                        _
//                        / ____|                      (_)
//                       | (___   ___  __ _ _   _  ___  _  __ _
//                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
//                        ____) |  __/ (_| | |_| | (_) | | (_| |
//                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine
//                                       | |
//                                       |_|
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Core/ErrorHandler.h"
#include "sequoia/Core/Options.h"
#include "sequoia/Core/StringRef.h"
#include "sequoia/Core/Version.h"
#include "sequoia/Driver/CommandLine.h"
#include <boost/program_options.hpp>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

namespace po = boost::program_options;

namespace sequoia {

namespace driver {

static void printHelp(const po::options_description& desc) {
  auto program = ErrorHandler::getSingleton().program().toAnsiString();
  std::cout << "OVERVIEW: Sequoia - 3D game engine\n\n";
  std::cout << "USAGE: " << program << " [options] \n\n" << desc << "\n" << std::endl;
  std::exit(EXIT_SUCCESS);
}

static void printVersion() {
  std::cout << "Sequoia (" << SEQUOIA_VERSION_STRING << ")" << std::endl;
  std::exit(EXIT_SUCCESS);
}

template <class ValueType>
void setOption(po::options_description& optionsDesc, const std::string& doc, StringRef cl,
               StringRef clMetaVar) {
  auto docStr = doc + ".";
  if(std::is_same<ValueType, bool>::value)
    optionsDesc.add_options()(cl.data(), docStr.c_str());
  else {
    auto value = po::value<ValueType>();
    if(!clMetaVar.empty())
      value->value_name(clMetaVar.data());
    optionsDesc.add_options()(cl.data(), value, docStr.c_str());
  }
}

template <class T>
T applyOption(const po::variable_value& value) {
  return value.as<T>();
}

template <>
bool applyOption(const po::variable_value& value) {
  return true;
}

template <class T>
std::string toString(const T& value) {
  return std::to_string(value);
}

template <>
std::string toString(const std::string& value) {
  return value;
}

void CommandLine::parse(const std::vector<std::string>& args) {
  const int MaxLineLen = 80;
  Options& options = Options::getSingleton();

  // Create option description maps & fill options
  std::unordered_map<std::string, po::options_description> optionDescMap;
  std::unordered_map<std::string, std::function<void(const po::variable_value&)>> updateOptionMap;

#define OPT(Structure, Name, Type, DefaultValue, CheckFun, Doc, CommandLine, CommandLineMetaVar)   \
  if(!optionDescMap.count(#Structure))                                                             \
    optionDescMap.emplace(#Structure, po::options_description(#Structure " options", MaxLineLen)); \
  if(!StringRef(CommandLine).empty()) {                                                            \
    setOption<Type>(optionDescMap[#Structure], Doc, CommandLine, CommandLineMetaVar);              \
    updateOptionMap.emplace(CommandLine, [&options](const po::variable_value& value) {             \
      options.Structure.Name = applyOption<Type>(value);                                           \
    });                                                                                            \
  }
#include "sequoia/Core/Options.inc"
#undef OPT

  po::options_description desc("General options");
  desc.add_options()
      // --help
      ("help", "Display this information.")
      // --version
      ("version", "Display version information.");

  for(const auto& nameOptionDescPair : optionDescMap)
    desc.add(nameOptionDescPair.second);

  // Parse command-line
  po::variables_map vm;

  try {
    po::store(po::command_line_parser(args).options(desc).run(), vm);
    po::notify(vm);
  } catch(std::exception& e) {
    ErrorHandler::getSingleton().fatal(e.what(), false);
  }

  // Adjust options
  if(vm.count("help"))
    printHelp(desc);

  if(vm.count("version"))
    printVersion();

  for(const auto& nameValuePair : vm) {
    auto it = updateOptionMap.find(nameValuePair.first);
    if(it != updateOptionMap.end())
      it->second(nameValuePair.second);
  }

// Check options are valid
#define OPT(Structure, Name, Type, DefaultValue, CheckFun, Doc, CommandLine, CommandLineMetaVar)   \
  if(!CheckFun(options.Structure.Name))                                                            \
    ErrorHandler::getSingleton().fatal(std::string("value '") + toString(options.Structure.Name) + \
                                           "' of option '--" + CommandLine + "' is invalid",       \
                                       false);
#include "sequoia/Core/Options.inc"
#undef OPT
}

} // namespace driver

} // namespace sequoia
