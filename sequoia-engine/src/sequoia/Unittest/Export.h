//===-- sequoia/Unittest/Export.h ---------------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
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