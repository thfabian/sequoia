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

#ifndef SEQUOIA_ENGINE_MATH_EXPORT_H
#define SEQUOIA_ENGINE_MATH_EXPORT_H

#ifdef SEQUOIA_DOXYGEN_INVOKED
/// @defgroup math Math
/// @brief Math and linear algebra utilities.

namespace sequoia {

/// @namespace math
/// @brief Namespace of the math library.
namespace math {}
}
#endif

#include "sequoia-engine/Core/Compiler.h"

#if defined(SEQUOIA_SHARED_LIBRARIES) && defined(SequoiaMath_EXPORTS)
#define SEQUOIA_API SEQUOIA_API_EXPORT
#else
#define SEQUOIA_API SEQUOIA_API_IMPORT
#endif

#endif