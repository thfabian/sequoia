//===-- sequoia/Driver/CommandLine.h ------------------------------------------------*- C++ -*-===//
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

#ifndef SEQUOIA_DRIVER_COMMANDLINE_H
#define SEQUOIA_DRIVER_COMMANDLINE_H

#include "sequoia/Driver/Export.h"
#include <string>
#include <vector>

namespace sequoia {

namespace driver {

/// @brief Parse command-line arguments and store them in `GlobalConfiguration`
///
/// @ingroup driver
class SEQUOIA_DRIVER_EXPORT CommandLine {
public:
  /// @brief Parse arguments and exit on error
  static void parse(const std::vector<std::string>& args);
};

} // namespace driver

} // namespace sequoia

#endif
