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
#include "sequoia-engine/Core/Options.h"
#include <string>
#include <vector>

namespace sequoia {

namespace driver {

/// @brief Parse command-line arguments and update the `Options`
/// @ingroup driver
class SEQUOIA_API CommandLine {
public:
  CommandLine() = delete;

  /// @brief Parse arguments and exit on error
  ///
  /// @param args       Arguments to parse
  /// @param options    Options to update
  static void parse(const std::vector<std::string>& args, Options* options);
};

} // namespace driver

} // namespace sequoia

#endif