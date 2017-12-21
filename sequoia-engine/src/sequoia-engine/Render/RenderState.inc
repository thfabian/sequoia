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

// clang-format off

#if !defined(RENDER_STATE)
#error "RENDER_STATE not defined"
#endif

// RENDER_STATE(Type, Name, DefaultValue)
//
//  - Type            = Type of the state
//  - Name            = Name of the state
//  - DefaultValue    = Value used to initialize the state
//
 
RENDER_STATE(bool, DepthTest, true)
RENDER_STATE(RenderPipeline::DepthFuncKind, DepthFunc, RenderPipeline::DF_Less)

// clang-format on
