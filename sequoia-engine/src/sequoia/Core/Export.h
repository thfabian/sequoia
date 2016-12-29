//===-- sequoia/Core/Export.h -------------------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT). 
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#ifndef SEQUOIA_CORE_EXPORT_H
#define SEQUOIA_CORE_EXPORT_H

#ifdef SEQUOIA_DOXYGEN_INVOKED
/// @defgroup core Core
/// @brief Core infrastructure of Sequoia.

/// @namespace sequoia
/// @brief Namespace of the sequoia project.
namespace sequoia {
  /// @namespace core
  /// @brief Namespace containing the core library.
  namespace core {}
}
#endif

#include "sequoia/Core/Config.h"

#if defined(SEQUOIA_ON_WIN32)
#if defined(SEQUOIA_SHARED_LIBRARIES) && defined(SequoiaCore_EXPORTS) 
#define SEQUOIA_CORE_EXPORT __declspec(dllexport)
#else
#define SEQUOIA_CORE_EXPORT __declspec(dllimport)
#endif
#else
#define SEQUOIA_CORE_EXPORT
#endif

#endif
