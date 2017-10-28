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

#include "sequoia-engine/Unittest/TestOptions.h"

namespace sequoia {

SEQUOIA_DECLARE_SINGLETON(unittest::TestOptions);

namespace unittest {

TestOptions::TestOptions() : Options() {}

void TestOptions::save() {
  Options opt;
#define OPT(Structure, Name, Type, DefaultValue, CheckFun, Doc, CommandLine, CommandLineShort,     \
            CommandLineMetaVar)                                                                    \
  opt.Structure.Name = this->Structure.Name;
#include "sequoia-engine/Core/Options.inc"
#undef OPT
  snapshots_.push(std::move(opt));
}

void TestOptions::load() {
  if(snapshots_.empty())
    return;

#define OPT(Structure, Name, Type, DefaultValue, CheckFun, Doc, CommandLine, CommandLineShort,     \
            CommandLineMetaVar)                                                                    \
  this->Structure.Name = snapshots_.top().Structure.Name;
#include "sequoia-engine/Core/Options.inc"
#undef OPT
  snapshots_.pop();
}

void TestOptions::restoreFirstSnapshot() {
  if(snapshots_.empty())
    return;

  while(snapshots_.size() != 1)
    snapshots_.pop();

  load();
}

} // namespace unittest

} // namespace sequoia
