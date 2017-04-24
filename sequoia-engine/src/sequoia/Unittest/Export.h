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

#ifndef SEQUOIA_UNITTEST_EXPORT_H
#define SEQUOIA_UNITTEST_EXPORT_H

#include "sequoia/Core/Config.h"

#if defined(SEQUOIA_ON_WIN32)
#if defined(SEQUOIA_SHARED_LIBRARIES) && defined(SequoiaUnittest_EXPORTS)
#define SEQUOIA_UNITTEST_EXPORT __declspec(dllexport)
#else
#define SEQUOIA_UNITTEST_EXPORT __declspec(dllimport)
#endif
#else
#define SEQUOIA_UNITTEST_EXPORT
#endif

#endif
