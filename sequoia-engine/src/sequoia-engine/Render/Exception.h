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

#ifndef SEQUOIA_ENGINE_RENDER_EXCEPTION_H
#define SEQUOIA_ENGINE_RENDER_EXCEPTION_H

#include "sequoia-engine/Core/Exception.h"

namespace sequoia {

namespace render {

/// @class sequoia::render::RenderSystemException
/// @brief Exception thrown on render related errors
/// @ingroup render
SEQUOIA_DECLARE_EXCPETION(RenderSystemException)

} // namespace render

} // namespace sequoia

#endif
