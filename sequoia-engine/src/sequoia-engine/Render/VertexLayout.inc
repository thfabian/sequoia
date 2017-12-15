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

#if !defined(VERTEX_LAYOUT_TYPE)
#error "VERTEX_LAYOUT_TYPE not defined"
#endif

// VERTEX_LAYOUT_TYPE(Type, Name)
//
//  - Type  = C++ type
//  - Name  = Name of the type
//
 
VERTEX_LAYOUT_TYPE(std::uint8_t, UInt8)
VERTEX_LAYOUT_TYPE(float, Float32)

// clang-format on
