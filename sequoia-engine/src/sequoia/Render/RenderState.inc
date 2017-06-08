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

// clang-format off

// RENDER_STATE(Type, Name, Value)
//
//  - Type  = Type of the state
//  - Name  = Name of the state
//  - Value = Default value of the state
// 
RENDER_STATE(bool, DepthTest, true)
RENDER_STATE(RenderState::DepthFuncKind, DepthFunc, RenderState::DF_Less)

// clang-format on
