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

#ifndef SEQUOIA_ENGINE_RENDER_D3D12_D3D12INPUTSYSTEM_H
#define SEQUOIA_ENGINE_RENDER_D3D12_D3D12INPUTSYSTEM_H

#include "sequoia-engine/Core/Listenable.h"
#include "sequoia-engine/Render/Input.h"

namespace sequoia {

namespace render {

/// @brief Mock implemenation of an input system
/// @ingroup d3d12
class SEQUOIA_API D3D12InputSystem : public Listenable<KeyboardListener, MouseListener> {};

} // namespace render

} // namespace sequoia

#endif
