//===-- sequoia/Driver/CommandLine.cpp ----------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//
//
/// @file
/// Parse command-line arguments.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Core/ErrorHandler.h"
#include "sequoia/Core/Exception.h"
#include "sequoia/Core/GlobalConfiguration.h"
#include "sequoia/Core/Version.h"
#include "sequoia/Driver/CommandLine.h"
#include <boost/program_options.hpp>
#include <string>

namespace po = boost::program_options;

namespace sequoia {

namespace driver {

static void printHelp(const po::options_description& desc) {
  auto program = core::ErrorHandler::getSingleton().program().asUTF8();
  std::cout << "Usage: " << program << " [options] \n\n" << desc << "\n" << std::endl;
  std::exit(EXIT_SUCCESS);
}

static void printVersion() {
  std::cout << "Sequoia (" << core::Version::currentFullVersionString() << ")" << std::endl;
  std::exit(EXIT_SUCCESS);
}

void CommandLine::parse(const std::vector<DefaultString>& args) {

  //
  // Set options
  //
  po::options_description general("General options");
  general.add_options()
      // --help
      ("help", "Display this information.")
      // --help-module=module
      ("help-module", po::value<DefaultString>()->value_name("MODULE"),
       "Produce a help for a given module.")
      // --version
      ("version", "Display version information.")
      // --verbose
      ("verbose", "Enable verbose logging to console [default: OFF]")
      // --config
      ("config", po::value<DefaultString>()->value_name("PATH"),
       "Path to the global configuration file [default: " SEQUOIA_GLOBAL_CONFIG_PATH "]");

  po::options_description gui("GUI options");
  gui.add_options()
      // --display
      ("display", po::value<int>(), "X display to use.");

  po::options_description all("Allowed options");
  all.add(general).add(gui);

  //
  // Parse command-line
  //
  po::variables_map vm;

  try {
    po::store(po::basic_command_line_parser<DefaultChar>(args).options(all).run(), vm);
    po::notify(vm);
  } catch(std::exception& e) {
    core::ErrorHandler::getSingleton().fatal(e.what());
  }

  //
  // Add parsed option to global configuration
  //
  auto& config = core::GlobalConfiguration::getSingleton();
  config.addSkipNode("CommandLine");

  if(vm.count("help"))
    printHelp(all);

  if(vm.count("version"))
    printVersion();

  boost::filesystem::path configPath(CSTR(SEQUOIA_GLOBAL_CONFIG_PATH));
  if(vm.count("config"))
    configPath = vm["config"].as<DefaultString>();

  try {
    config.load(configPath);
    config.put("CommandLine.ConfigFile", configPath);
  } catch(core::Exception& e) {
    core::ErrorHandler::getSingleton().warning(e.what());
  }

  if(vm.count("verbose"))
    config.put("CommandLine.Verbose", true);

  if(vm.count("display"))
    config.put("CommandLine.Display", vm["display"].as<int>());
}

} // namespace driver
 
} // namespace sequoia
