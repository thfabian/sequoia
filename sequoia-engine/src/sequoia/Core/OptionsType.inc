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

#if !defined(OPTIONS_TYPE)
#error "OPTIONS_TYPE not defined"
#endif

// OPTIONS_TYPE(Type, Name)
//
//  - Type  = C++ type
//  - Name  = Name of the type
//
 
OPTIONS_TYPE(bool, Bool)
OPTIONS_TYPE(int, Int)
OPTIONS_TYPE(float, Float)
OPTIONS_TYPE(std::string, String)

// clang-format on
