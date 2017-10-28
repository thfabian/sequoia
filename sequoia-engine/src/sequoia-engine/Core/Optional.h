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

#ifndef SEQUOIA_CORE_OPTIONAL_H
#define SEQUOIA_CORE_OPTIONAL_H

#include "sequoia-engine/Core/Compiler.h"

#if SEQUOIA_HAS_CXX17 && __has_include(<optional>)
#define SEQUOIA_USE_STD_OPTIONAL
#include <optional>
#else
#include <boost/optional.hpp>
#endif

namespace sequoia {

namespace core {

/// @typedef optional
/// @brief Wrapper for representing 'optional' (or 'nullable') objects who may not (yet) contain a
/// valid value
/// @ingroup core
#ifdef SEQUOIA_USE_STD_OPTIONAL
template <class T>
using optional = std::optional<T>;
#else
template <class T>
using optional = boost::optional<T>;
#endif

} // namespace core

} // namespace sequoia

#endif
