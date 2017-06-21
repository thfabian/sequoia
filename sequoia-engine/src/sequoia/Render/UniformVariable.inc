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

// UNIFORM_VARIABLE_TYPE(Type, Enum, Name)
//
//  - Type  = C++ type
//  - Enum  = Eunum of the type (UniformType)
//  - Name  = Name of the type
//
 
UNIFORM_VARIABLE_TYPE(int, UniformType::Int, "Int")
UNIFORM_VARIABLE_TYPE(float, UniformType::Float, "Float")

// clang-format on
