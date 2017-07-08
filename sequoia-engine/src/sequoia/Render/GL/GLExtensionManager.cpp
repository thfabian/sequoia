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

#include "sequoia/Render/GL/GLExtensionManager.h"
#include <glbinding/ContextInfo.h>

namespace sequoia {

namespace render {

bool GLExtensionManager::isSupported(gl::GLextension extension) noexcept {
  SEQUOIA_LOCK_GUARD(mutex_);

  auto it = extensionCache_.find(extension);
  if(it != extensionCache_.end())
    return it->second;

  bool supported = glbinding::ContextInfo::supported({extension});
  extensionCache_.emplace(extension, supported);
  return supported;
}

} // render

} // namespace sequoia
