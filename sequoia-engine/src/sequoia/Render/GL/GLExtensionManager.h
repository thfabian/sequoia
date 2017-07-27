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

#ifndef SEQUOIA_RENDER_GL_GLEXTENSIONMANAGER_H
#define SEQUOIA_RENDER_GL_GLEXTENSIONMANAGER_H

#include "sequoia/Core/Export.h"
#include "sequoia/Core/Mutex.h"
#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Render/GL/GLFwd.h"
#include <unordered_map>

namespace sequoia {

namespace render {

/// @brief Manage OpenGL extensions
/// @ingroup gl
class SEQUOIA_API GLExtensionManager : public NonCopyable {
  
  /// Access mutex
  SpinMutex mutex_;

  /// Cache of queried extensions
  std::unordered_map<gl::GLextension, bool> extensionCache_;

public:
  /// @brief Check if `extension` is supported
  ///
  /// @returns `true` if extension is supported, `false` otherwise
  /// @remark Thread-safe
  bool isSupported(gl::GLextension extension) noexcept;
};

} // namespace render

} // namespace sequoia

#endif
