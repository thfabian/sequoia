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

#ifndef SEQUOIA_ENGINE_RENDER_D3D12_WIN32UTIL_H
#define SEQUOIA_ENGINE_RENDER_D3D12_WIN32UTIL_H

#include "sequoia-engine/Core/Export.h"
#include <string>

namespace sequoia {

namespace render {

/// @brief Utility function of the Win32 API
/// @ingroup d3d12
struct SEQUOIA_API Win32Util {
  Win32Util() = delete;

  /// @brief Retrieves the calling thread's last-error code (converted to a string)
  ///
  /// The last-error code is maintained on a per-thread basis. Multiple threads do not overwrite
  /// each
  /// other's last-error code.
  static std::string getLastError();
};

} // namespace render

} // namespace sequoia

#endif
