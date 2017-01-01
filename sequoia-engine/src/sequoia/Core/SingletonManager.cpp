//===-- sequoia/Core/SingletonManager.cpp -------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Core/SingletonManager.h"

template <>
sequoia::core::SingletonManager* Ogre::Singleton<sequoia::core::SingletonManager>::msSingleton =
    nullptr;
