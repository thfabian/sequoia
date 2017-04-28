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

#ifndef SEQUOIA_CORE_NPATH_H
#define SEQUOIA_CORE_NPATH_H

#include <boost/filesystem/path.hpp>

namespace sequoia {

namespace core {

/// @typedef NPath
/// @brief The native path of the platform with respect to `NString`
/// @ingroup core
using NPath = boost::filesystem::path;

} // namespace core

using NPath = core::NPath;

} // namespace sequoia

#endif
