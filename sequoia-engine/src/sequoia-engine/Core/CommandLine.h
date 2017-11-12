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

#ifndef SEQUOIA_ENGINE_CORE_COMMANDLINE_H
#define SEQUOIA_ENGINE_CORE_COMMANDLINE_H

#include "sequoia-engine/Core/Export.h"
#include <memory>
#include <string>
#include <vector>

#ifdef SEQUOIA_ON_WIN32
#include "sequoia-engine/Core/Platform.h"
#endif

namespace sequoia {

namespace core {

class Options;

/// @brief Parse command-line arguments and update the `Options`
/// @ingroup core
class SEQUOIA_API CommandLine {
public:
  /// @brief Set the command-line meta-data
  ///
  /// @param tool       Name of the tool
  /// @param version    Version string of the tool
  CommandLine(const std::string& tool, const std::string& version);

  /// @brief Parse arguments and update the options
  ///
  /// In addition to the options defined in `core::Options::getOptionsMetaData()`, there are two
  /// builtin command-line options "--help" and "--version" which display the help string and
  /// version string respectively and exit the program with `EXIT_SUCCESS`.
  /// Note that `core::ErrorHandler::fatal` is called in case an error occurs.
  ///
  /// @param options    Options to update
  /// @param argc       Number of arguments (length of `argv`)
  /// @param argv       String of arguments (as passed to `main()` - note that the first arguments
  ///                   is treated as the *name* of the program)
  void parse(Options* options, int argc, char* argv[]);
  void parse(const std::shared_ptr<Options>& options, int argc, char* argv[]) {
    parse(options.get(), argc, argv);
  }

#ifdef SEQUOIA_ON_WIN32
  /// @brief Parse arguments and update the options (Win32 entry point version)
  void parse(Options* options, HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,
             int nCmdShow);
  void parse(const std::shared_ptr<Options>& options, HINSTANCE hInstance, HINSTANCE hPrevInstance,
             LPSTR lpCmdLine, int nCmdShow) {
    parse(options.get(), hInstance, hPrevInstance, lpCmdLine, nCmdShow);
  }
#endif

private:
  void parseImpl(Options* options, const std::string& program,
                 const std::vector<std::string>& arguments);

private:
  std::string tool_;    ///< Name of the tool
  std::string version_; ///< Version string
};

} // namespace core

} // namespace sequoia

#endif
