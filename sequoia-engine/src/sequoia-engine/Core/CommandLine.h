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
#include "sequoia-engine/Core/Options2.h"
#include <string>
#include <vector>

namespace sequoia {

namespace core {

/// @brief Parse command-line arguments and update the `Options`
/// @ingroup core
class SEQUOIA_API CommandLine {
public:
  /// @brief Set the command-line meta-data
  ///
  /// @param program    Name (or path) to the program
  /// @param tool       Name of the tool
  /// @param version    Version string of the tool
  CommandLine(const std::string& program, const std::string& tool, const std::string& version);

  /// @brief Parse arguments and update the options
  ///
  /// In addition to the options defined in `core::Options::getOptionsMetaData()`, there are two
  /// builtin command-line options "--help" and "--version" which display the help string and
  /// version string respectively and exit the program with `EXIT_SUCCESS`.
  /// Note that `core::ErrorHandler::fatal` is called in case an error occurs.
  ///
  /// @param args       Arguments to parse
  /// @param options    Options to update
  void parse(const std::vector<std::string>& args, Options2* options);

private:
  std::string program_; ///< Name of the program
  std::string tool_;    ///< Name of the tool
  std::string version_; ///< Version string
};

} // namespace core

} // namespace sequoia

#endif
