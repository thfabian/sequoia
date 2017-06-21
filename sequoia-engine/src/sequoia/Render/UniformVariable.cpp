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

#include "sequoia/Core/Format.h"
#include "sequoia/Core/Unreachable.h"
#include "sequoia/Render/UniformVariable.h"
#include <ostream>
#include <sstream>

namespace sequoia {

namespace render {

namespace {

static std::string AnyString(const core::any& data, UniformType type) {
  std::stringstream ss;
  switch(type) {
#define UNIFORM_VARIABLE_TYPE(Type, Enum, Name)                                                    \
  case Enum:                                                                                       \
    ss << (*core::any_cast<Type>(&data));                                                          \
    break;
#include "sequoia/Render/UniformVariable.inc"
#undef UNIFORM_VARIABLE_TYPE
  default:
    sequoia_unreachable("invalid type");
  }
  return ss.str();
}

} // anonymous namespace

std::ostream& operator<<(std::ostream& os, UniformType type) {
  switch(type) {
#define UNIFORM_VARIABLE_TYPE(Type, Enum, Name)                                                    \
  case Enum:                                                                                       \
    os << Name;                                                                                    \
    break;
#include "sequoia/Render/UniformVariable.inc"
#undef UNIFORM_VARIABLE_TYPE
  default:
    sequoia_unreachable("invalid type");
  }
  return os;
}

std::string UniformVariable::toString() const {
  return core::format("UniformVariable[\n"
                      "  name = %s,\n"
                      "  type = %s,\n"
                      "  data = %s\n"
                      "]",
                      name_, type_, AnyString(data_, type_));
}

} // namespace render

} // namespace sequoia
