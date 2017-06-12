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

#ifndef SEQUOIA_CORE_ALIGNEDADT_H
#define SEQUOIA_CORE_ALIGNEDADT_H

#include "sequoia/Core/Memory.h"
#include <vector>

namespace sequoia {

namespace core {

/// @addtogroup core
/// @{

/// @brief Aligned version of `std::vector`
template <class T>
using aligned_vector = std::vector<T, memory::aligned_allocator<T>>;

/// @}

} // namespace core

template <class T>
using aligned_vector = core::aligned_vector<T>;

} // namespace sequoia

#endif
