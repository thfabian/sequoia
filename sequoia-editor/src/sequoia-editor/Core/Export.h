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

#ifndef SEQUOIA_EDITOR_CORE_EXPORT_H
#define SEQUOIA_EDITOR_CORE_EXPORT_H

#ifdef SEQUOIA_EDITOR_DOXYGEN_INVOKED

/// @defgroup core Core
/// @brief Core and utility functionality.

/// @defgroup driver Driver
/// @brief Driver infrastructure.

/// @namespace sequoia
/// @brief Namespace of the sequoia-editor project.
namespace sequoia_editor {

/// @namespace core
/// @brief Namespace of the core library.
namespace core {}

} // namespace sequoia

#endif

#include "sequoia-engine/Core/Compiler.h"

#if defined(SEQUOIA_EDITOR_SHARED_LIBRARIES)
#define SEQUOIA_EDITOR_API SEQUOIA_API_EXPORT
#else
#define SEQUOIA_EDITOR_API
#endif

#endif
