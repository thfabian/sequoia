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

#include "sequoia/Core/StringUtil.h"

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

} // namespace core

} // namespace sequoia
