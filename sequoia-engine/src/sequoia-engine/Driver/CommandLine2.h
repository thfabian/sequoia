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

#ifndef SEQUOIA_ENGINE_DRIVER_COMMANDLINE_H
#define SEQUOIA_ENGINE_DRIVER_COMMANDLINE_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/Options2.h"
#include <string>
#include <vector>

namespace sequoia {

namespace driver {

/// @brief Parse command-line arguments and update the `Options`
/// @ingroup driver
class SEQUOIA_API CommandLine2 {
public:
  CommandLine2(const std::string& tool, const std::string& version);

  /// @brief Parse arguments and exit on error
  ///
  /// Note that the `core::ErrorHandler` needs to be set up to handle failures correctly.
  ///
  /// @param args       Arguments to parse
  /// @param options    Options to update
  void parse(const std::vector<std::string>& args, Options2* options);

private:
  std::string tool_;    ///< Name of the tool
  std::string version_; ///< Version string
};

} // namespace driver

} // namespace sequoia

#endif
