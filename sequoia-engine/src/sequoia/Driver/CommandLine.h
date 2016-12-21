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

#include "sequoia/Core/Core.h"
#include <string>
#include <vector>

namespace sequoia {

namespace driver {

/// @brief Parse command-line arguments and store them in `GlobalConfiguration`
///
/// @ingroup driver
class SEQUOIA_EXPORT CommandLine {
public:
#ifdef SEQUOIA_ON_WIN32
  using StringType = std::wstring;
#else
  using StringType = std::string;
#endif

  using VectorType = std::vector<StringType>;

  /// @brief Parse arguments and exit on error
  static void parse(const std::vector<StringType>& arguments);
};

} // namespace driver

} // namespace sequoia

#endif
