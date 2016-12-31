//===-- sequoia/Driver/CommandLine.cpp ----------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
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

void CommandLine::parse(const std::vector<std::string>& args) {

  //
  // Set options
  //
  po::options_description general("General options");
  general.add_options()
      // --help
      ("help", "Display this information.")
      // --help-module=module
      ("help-module", po::value<std::string>()->value_name("MODULE"),
       "Produce a help for a given module.")
      // --version
      ("version", "Display version information.")
      // --verbose
      ("verbose", "Enable verbose logging to console [default: OFF]")
      // --config
      ("config", po::value<std::string>()->value_name("PATH"),
       "Path to the global configuration file [default: " SEQUOIA_GLOBAL_CONFIG_PATH "]");

  po::options_description gui("Graphics options");
  gui.add_options()
      // --display
      ("render-system", po::value<std::string>(), "Rendering system to use: Direct3D11 or OpenGL.");

  po::options_description all("Allowed options");
  all.add(general).add(gui);

  //
  // Parse command-line
  //
  po::variables_map vm;

  try {
    po::store(po::command_line_parser(args).options(all).run(), vm);
    po::notify(vm);
  } catch(std::exception& e) {
    core::ErrorHandler::getSingleton().fatal(e.what(), false);
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
    configPath = vm["config"].as<std::string>();

  try {
    config.load(configPath);
    config.put("CommandLine.ConfigFile", configPath);
  } catch(core::Exception& e) {
    core::ErrorHandler::getSingleton().warning(e.what());
  }

  if(vm.count("verbose"))
    config.put("CommandLine.Verbose", true);

  if(vm.count("render-system"))
    config.put("CommandLine.RenderSystem", vm["render-system"].as<std::string>());


}

} // namespace driver

} // namespace sequoia
