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

#ifndef SEQUOIA_ENGINE_CORE_XML_H
#define SEQUOIA_ENGINE_CORE_XML_H

#include "sequoia-engine/Core/StringUtil.h"
#include <pugixml.hpp>

namespace sequoia {

namespace core {

using namespace pugi;

/// @brief Append a child node to `node` which contains `value`
/// @return `false` if node is empty, there is not enough memory, or node can not have name/value
/// @ingroup core
template <class T>
bool xml_set_value(xml_node& child, T&& value) {
  return child.append_child(node_pcdata).set_value(core::toString(value).c_str());
}

/// @brief Get the default indentation for writing xml documents
inline constexpr const char* xml_indent() noexcept { return "  "; }

} // namespace core

} // namespace sequoia

#endif
