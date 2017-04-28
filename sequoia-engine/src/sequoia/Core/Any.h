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

#ifndef SEQUOIA_CORE_ANY_H
#define SEQUOIA_CORE_ANY_H

#include <boost/any.hpp>

namespace sequoia {

namespace core {

/// @typedef any
/// @brief Type-safe container for single values of *any* type
/// @ingroup core
using boost::any;
using boost::any_cast;

} // namespace core

} // namespace sequoia

#endif
