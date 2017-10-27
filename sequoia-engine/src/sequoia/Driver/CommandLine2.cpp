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

#include "sequoia/Core/ErrorHandler.h"
#include "sequoia/Driver/CommandLine2.h"
#include <boost/program_options.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>

namespace po = boost::program_options;

namespace sequoia {

namespace driver {

static void printHelp(const std::string& tool, const po::options_description& desc) {
  auto program = ErrorHandler::getSingleton().program().toAnsiString();
  std::cout << "OVERVIEW: " << tool << "\n\n";
  std::cout << "USAGE: " << program << " [options] \n\n" << desc << "\n" << std::endl;
  std::exit(EXIT_SUCCESS);
}

static void printVersion(const std::string& tool, const std::string& version) {
  std::cout << tool << "(" << version << ")" << std::endl;
  std::exit(EXIT_SUCCESS);
}

CommandLine2::CommandLine2(const std::string& tool, const std::string& version)
    : tool_(tool), version_(version) {}

void CommandLine2::parse(const std::vector<std::string>& args, Options2* options) {
  const int MaxLineLen = 80;

  const std::unordered_map<std::string, core::OptionMetaData>& optionMetaDataMap =
      options->getOptionsMetaData();

  // Create option description maps & fill options
  std::unordered_map<std::string, po::options_description> optionDescMap;
  std::unordered_map<std::string, std::string> commandLineToOptionMap;

  for(const auto& optionPair : optionMetaDataMap) {
    const std::string& option = optionPair.first;
    const core::OptionMetaData& metaData = optionPair.second;

    std::size_t idx = option.find_first_of('.');
    std::string base = option.substr(0, idx);

    if(!optionDescMap.count(base))
      optionDescMap.emplace(base, po::options_description(base + " options", MaxLineLen));

    auto docStr = metaData.DocString + ".";
    auto cl = metaData.CommandLine +
              (metaData.CommandLineShort.empty() ? "" : ("," + metaData.CommandLineShort));

    po::options_description& optionsDesc = optionDescMap[base];

    if(metaData.CommandLineHasValue) {
      optionsDesc.add_options()(cl.c_str(), docStr.c_str());
    } else {
      auto value = po::value<std::string>();
      if(!metaData.CommandLineMetaVar.empty())
        value->value_name(metaData.CommandLineMetaVar);
      optionsDesc.add_options()(cl.c_str(), value, docStr.c_str());
    }

    commandLineToOptionMap[option] = metaData.CommandLine;
  }

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
    ErrorHandler::getSingleton().fatal(e.what(), false, false);
  }

  if(vm.count("help"))
    printHelp(tool_, desc);

  if(vm.count("version"))
    printVersion(tool_, version_);

  for(const auto& nameValuePair : vm) {
    const std::string& optionName = commandLineToOptionMap[nameValuePair.first];
    options->set(optionName, nameValuePair.second.as<std::string>());
  }
}

} // namespace driver

} // namespace sequoia
