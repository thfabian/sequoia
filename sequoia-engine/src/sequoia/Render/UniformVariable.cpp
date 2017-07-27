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

#include "sequoia/Core/Format.h"
#include "sequoia/Core/Unreachable.h"
#include "sequoia/Render/UniformVariable.h"
#include <ostream>
#include <sstream>

namespace sequoia {

namespace render {

namespace {

static std::string VariantToString(const UniformVariable::DataType& data, UniformType type) {
  std::stringstream ss;
  switch(type) {
#define UNIFORM_VARIABLE_TYPE(Type, Name)                                                          \
  case UniformType::Name:                                                                          \
    ss << boost::get<Type>(data);                                                                  \
    break;
#include "sequoia/Render/UniformVariable.inc"
#undef UNIFORM_VARIABLE_TYPE
  case UniformType::Invalid:
    ss << "<invalid>";
    break;
  default:
    sequoia_unreachable("invalid type");
  }
  return ss.str();
}

} // anonymous namespace

std::ostream& operator<<(std::ostream& os, UniformType type) {
  switch(type) {
#define UNIFORM_VARIABLE_TYPE(Type, Name)                                                          \
  case UniformType::Name:                                                                          \
    os << #Name;                                                                                   \
    break;
#include "sequoia/Render/UniformVariable.inc"
#undef UNIFORM_VARIABLE_TYPE
  case UniformType::Invalid:
    os << "Invalid";
    break;
  default:
    sequoia_unreachable("invalid type");
  }
  return os;
}

bool UniformVariable::operator==(const UniformVariable& other) const noexcept {
  if(type_ != other.type_)
    return false;

  switch(type_) {
#define UNIFORM_VARIABLE_TYPE(Type, Name)                                                          \
  case UniformType::Name:                                                                          \
    return boost::get<Type>(data_) == boost::get<Type>(other.data_);
#include "sequoia/Render/UniformVariable.inc"
#undef UNIFORM_VARIABLE_TYPE
  case UniformType::Invalid:
    return boost::get<InvalidData>(data_) == boost::get<InvalidData>(other.data_);
  default:
    sequoia_unreachable("invalid type");
  }
}

std::string UniformVariable::toString() const {
  return core::format("UniformVariable[\n"
                      "  type = %s,\n"
                      "  data = %s\n"
                      "]",
                      type_, VariantToString(data_, type_));
}

std::ostream& operator<<(std::ostream& os, const UniformVariable& var) {
  return (os << var.toString());
}

} // namespace render

} // namespace sequoia
