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

#include "sequoia/Render/RenderRessource.h"

namespace sequoia {

namespace render {

void RenderRessource::makeValid() {
  if(isValid())
    return;

  SEQUOIA_LOCK_GUARD(mutex_);

  // Prevent the ressource from being initialized multiple times or repeat the error if it already
  // failed
  if(isValid() || hasException())
    return;

  // Initialize the ressource and store the exceptions
  try {
    makeValidImpl();
  } catch(...) {
    exception_ = std::current_exception();
    return;
  }

  valid_.store(true);
}

void RenderRessource::rethrowException() {
  SEQUOIA_LOCK_GUARD(mutex_);
  if(exception_)
    std::rethrow_exception(exception_);
}



} // namespace render

} // namespace sequoia
