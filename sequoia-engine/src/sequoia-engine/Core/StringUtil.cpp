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

#include "sequoia-engine/Core/StringUtil.h"
#include "sequoia-engine/Core/Timer.h"

namespace sequoia {

namespace core {

std::string indent(const std::string& string, int amount) {
  // This could probably be done faster (it's not really speed-critical though)
  std::istringstream iss(string);
  std::ostringstream oss;
  std::string spacer(amount, ' ');
  bool firstLine = true;
  for(std::string line; std::getline(iss, line);) {
    if(!firstLine)
      oss << spacer;
    oss << line;
    if(!iss.eof())
      oss << "\n";
    firstLine = false;
  }
  return oss.str();
}

void forEachLine(const std::string& string, std::function<void(StringRef)> functor) {
  std::size_t begin = 0, pos = 0;
  for(; pos < string.size(); ++pos) {
    if(string[pos] == '\n') {
      functor(StringRef(string.c_str() + begin, pos - begin));
      begin = pos + 1;
    }
  }
  functor(StringRef(string.c_str() + begin, pos - begin));
}

std::string timeString(double time, bool precise) {
  if(std::isnan(time) || std::isinf(time))
    return "inf";

  std::string suffix = "ms";
  if(time > 1000) {
    time /= 1000;
    suffix = "s";
    if(time > 60) {
      time /= 60;
      suffix = "m";
      if(time > 60) {
        time /= 60;
        suffix = "h";
        if(time > 24) {
          time /= 24;
          suffix = "d";
        }
      }
    }
  }

  if(precise)
    return core::format("{:.4f} {}", time, suffix);
  else
    return core::format("{:.1f} {}", time, suffix);
}

} // namespace core

} // namespace sequoia
