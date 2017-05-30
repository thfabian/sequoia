//===--------------------------------------------------------------------------------*- C++ -*-===//
//                         _____                        _
//                        / ____|                      (_)
//                       | (___   ___  __ _ _   _  ___  _  __ _
//                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
//                        ____) |  __/ (_| | |_| | (_) | | (_| |
//                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine
//                                       | |
//                                       |_|
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Core/Compiler.h"
#include "sequoia/Core/PrettyStackTrace.h"

#ifdef SEQUOIA_ON_LINUX

#if defined(SEQUOIA_BACKWARD_HAS_DW)
#define BACKWARD_HAS_DW 1
#elif defined(SEQUOIA_BACKWARD_HAS_BFD)
#define BACKWARD_HAS_BFD 1
#endif

#include <backward/backward.hpp>
#endif

namespace sequoia {

SEQUOIA_DECLARE_SINGLETON(core::PrettyStackTrace);

namespace core {

#ifdef SEQUOIA_ON_LINUX

class PrettyStackTrace::Impl {
  backward::SignalHandling sh;
};

PrettyStackTrace::PrettyStackTrace()
#if defined(SEQUOIA_DISABLE_ASSERTS) || defined(NDEBUG)
    : impl_(nullptr)
#else
    : impl_(new Impl) {
#endif
}

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
