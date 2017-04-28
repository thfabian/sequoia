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

#ifndef SEQUOIA_GRAPHICS_EXPORT_H
#define SEQUOIA_GRAPHICS_EXPORT_H

#ifdef SEQUOIA_DOXYGEN_INVOKED
/// @defgroup graphics Graphics
/// @brief Graphics and rendering

/// @namespace sequoia
/// @brief Namespace of the sequoia project.
namespace sequoia {
/// @namespace graphics
/// @brief Namespace of the graphics library.
namespace graphics {}
}
#endif

#include "sequoia/Core/Compiler.h"

#if defined(SEQUOIA_SHARED_LIBRARIES) && defined(SequoiaGraphics_EXPORTS)
#define SEQUOIA_GRAPHICS_API SEQUOIA_API_EXPORT
#else
#define SEQUOIA_GRAPHICS_API SEQUOIA_API_IMPORT
#endif

#endif
