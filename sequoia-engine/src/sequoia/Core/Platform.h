/*===-- sequoia/core/Platform.h -----------------------------------------------------*- C++ -*-===*\
 *
 *                                      S E Q U O I A
 *
 * This file is distributed under the MIT License (MIT). 
 * See LICENSE.txt for details.
 *
 *===------------------------------------------------------------------------------------------===//
 *
 *! @file
 *! Platfrom specific definitions.
 *!
 *! This is a C compliant header file.
 *
\*===------------------------------------------------------------------------------------------===*/

#ifndef SEQUOIA_CORE_PLATFORM_H
#define SEQUOIA_CORE_PLATFORM_H

#include "sequoia/Core/Config.h"

/*! @macro SEQUOIA_EXPORT
 *! @brief Export data, functions, classes, or class member functions from a DLL
 *! 
 *! Only affects Win32 builds.
 */
#if defined(SEQUOIA_ON_WIN32)
#ifdef SEQUOIA_SHARED_LIBRARIES
#define SEQUOIA_EXPORT __declspec(dllexport)
#else
#define SEQUOIA_EXPORT __declspec(dllimport)
#endif
#else
#define SEQUOIA_EXPORT
#endif

#endif
