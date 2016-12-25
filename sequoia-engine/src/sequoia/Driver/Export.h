//===-- sequoia/Driver/Export.h -----------------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT). 
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#ifndef SEQUOIA_DRIVER_EXPORT_H
#define SEQUOIA_DRIVER_EXPORT_H

#include "sequoia/Core/Config.h"

#if defined(SEQUOIA_ON_WIN32)
#if defined(SEQUOIA_SHARED_LIBRARIES) && defined(SequoiaDriver_EXPORTS)
#define SEQUOIA_DRIVER_EXPORT __declspec(dllexport)
#else
#define SEQUOIA_DRIVER_EXPORT __declspec(dllimport)
#endif
#else
#define SEQUOIA_DRIVER_EXPORT
#endif

#endif
