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

#include "sequoia-engine/Core/PrettyStackTrace.h"
#include "sequoia-engine/Core/Compiler.h"

#ifdef SEQUOIA_ON_LINUX
#include <backward.hpp>
#endif

namespace sequoia {

SEQUOIA_DECLARE_SINGLETON(core::PrettyStackTrace);

namespace core {

#ifdef SEQUOIA_ON_LINUX

class PrettyStackTrace::Impl {
  backward::SignalHandling sh;
};

PrettyStackTrace::PrettyStackTrace()
#ifdef SEQUOIA_DISABLE_ASSERTS
    : impl_(nullptr) {
}
#else
    : impl_(new Impl) {
}
#endif

PrettyStackTrace::~PrettyStackTrace() {
  if(impl_)
    delete impl_;
}

#else

class PrettyStackTrace::Impl {};

PrettyStackTrace::PrettyStackTrace() : impl_(nullptr) { (void)impl_; }
PrettyStackTrace::~PrettyStackTrace() {}

#endif

} // namespace core

} // namespace sequoia
