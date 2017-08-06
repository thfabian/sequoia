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

#ifndef SEQUOIA_CORE_ANY_H
#define SEQUOIA_CORE_ANY_H

#include "sequoia/Core/Compiler.h"

#if SEQUOIA_HAS_CXX17 && __has_include(<any>)
#define SEQUOIA_USE_STD_ANY
#include <any>
#else
#include <boost/any.hpp>
#endif

namespace sequoia {

namespace core {

/// @typedef any
/// @brief Type-safe container for single values of *any* type
/// @ingroup core
#ifdef SEQUOIA_USE_STD_ANY
using std::any;
using std::any_cast;
#else
using boost::any;
using boost::any_cast;
#endif

} // namespace core

} // namespace sequoia

#endif
