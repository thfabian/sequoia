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

#ifndef SEQUOIA_CORE_EXPORT_H
#define SEQUOIA_CORE_EXPORT_H

#ifdef SEQUOIA_DOXYGEN_INVOKED

/// @defgroup core Core
/// @brief Core infrastructure of Sequoia.

/// @defgroup driver Driver
/// @brief Driver infrastructure of Sequoia.

/// @defgroup game Game
/// @brief Main game infrastructure of Sequoia.

/// @defgroup math Math
/// @brief Math and linear algebra utilities.

/// @defgroup render Render
/// @brief Rendering and input devices.

/// @defgroup unittest Unittest
/// @brief Unittest infrastructure of Sequoia.

/// @namespace sequoia
/// @brief Namespace of the sequoia project.
namespace sequoia {

/// @namespace core
/// @brief Namespace of the core library.
namespace core {}

/// @namespace platform
/// @brief Platform specific implementations and typedefs
namespace platform {}

/// @namespace driver
/// @brief Namespace containing the driver library.
namespace driver {}

/// @namespace game
/// @brief Namespace containing the driver library.
namespace game {}

/// @namespace render
/// @brief Namespace of the rendering library.
namespace render {}

/// @namespace unittest
/// @brief Namespace of the unittest library.
namespace unittest {}

} // namespace sequoia

#endif

#include "sequoia/Core/Compiler.h"

#if defined(SEQUOIA_SHARED_LIBRARIES)
#define SEQUOIA_API SEQUOIA_API_EXPORT
#else
#define SEQUOIA_API SEQUOIA_API_IMPORT
#endif

#endif
