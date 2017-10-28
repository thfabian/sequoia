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

#if !defined(UNIFORM_VARIABLE_TYPE)
#error "UNIFORM_VARIABLE_TYPE not defined"
#endif

// UNIFORM_VARIABLE_TYPE(Type, Name)
//
//  - Type  = C++ type
//  - Name  = Name of the type
//
 
UNIFORM_VARIABLE_TYPE(bool, Bool)
UNIFORM_VARIABLE_TYPE(int, Int)
UNIFORM_VARIABLE_TYPE(float, Float)
UNIFORM_VARIABLE_TYPE(math::fvec2, Float2)
UNIFORM_VARIABLE_TYPE(math::fvec3, Float3)
UNIFORM_VARIABLE_TYPE(math::fvec4, Float4)
UNIFORM_VARIABLE_TYPE(math::fmat2, Float2x2)
UNIFORM_VARIABLE_TYPE(math::fmat3, Float3x3)
UNIFORM_VARIABLE_TYPE(math::fmat4, Float4x4)

// clang-format on
