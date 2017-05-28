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

#ifndef SEQUOIA_RENDER_GL_GLSTATECACHE_H
#define SEQUOIA_RENDER_GL_GLSTATECACHE_H

#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Render/Export.h"
#include <unordered_map>

namespace sequoia {

namespace render {

/// @brief Manager of the OpenGL state-machine
/// @ingroup gl
class SEQUOIA_RENDER_API GLStateCacheManager : public NonCopyable {
public:

};

} // namespace render

} // namespace sequoia

#endif
