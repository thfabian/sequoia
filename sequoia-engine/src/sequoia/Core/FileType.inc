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

#if !defined(FILE_TYPE)
#error "FILE_TYPE not defined"
#endif

// FILE_TYPE(Name, IsBinary, Extensions)
//
//  - Name            = Name of the file-type
//  - IsBinary        = Is the associated file-type a binary format?
//  - Extension       = List of of extensions, separated by '|'
//
 
FILE_TYPE(Text, false, ".txt")
FILE_TYPE(Shader, false, ".frag|.vert|.geom|.tess")
FILE_TYPE(Lua, false, ".lua")
FILE_TYPE(Png, true, ".png")
FILE_TYPE(Jpeg, true, ".jpg|.jpeg|.jpe")
FILE_TYPE(Bmp, true, ".bmp|.dib")

// clang-format on
