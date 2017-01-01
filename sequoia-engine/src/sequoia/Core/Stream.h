//===-- sequoia/Core/Stream.h -------------------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#ifndef SEQUOIA_CORE_STREAM_H
#define SEQUOIA_CORE_STREAM_H

#include "sequoia/Core/Core.h"
#include <iosfwd>

namespace sequoia {

namespace core {

/// @class DefaultOStream
/// @brief ostream suited for `DefaultString`
/// @ingroup core

/// @class DefaultIFStream
/// @brief input file stream suited for `DefaultString`
/// @ingroup core

/// @class DefaultOFStream
/// @brief output file streamsuited for `DefaultString`
/// @ingroup core

/// @class DefaultStringStream
/// @brief string stream suited for `DefaultString`
/// @ingroup core

#ifdef SEQUOIA_ON_WIN32
using DefaultOStream = std::wostream;
using DefaultIFStream = std::wifstream;
using DefaultOFStream = std::wofstream;
using DefaultStringStream = std::wstringstream;
#else
using DefaultOStream = std::ostream;
using DefaultIFStream = std::ifstream;
using DefaultOFStream = std::ofstream;
using DefaultStringStream = std::stringstream;
#endif

} // namespace core

using DefaultOStream = core::DefaultOStream;
using DefaultIFStream = core::DefaultIFStream;
using DefaultOFStream = core::DefaultOFStream;
using DefaultStringStream = core::DefaultStringStream;

} // namespace sequoia

#endif
