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

#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Core/StringUtil.h"
#include "sequoia-engine/Core/Unreachable.h"
#include "sequoia-engine/Render/UniformVariable.h"
#include <boost/preprocessor/stringize.hpp>
#include <ostream>
#include <sstream>

namespace sequoia {

namespace render {

namespace {

static std::string variantToString(const UniformVariable::DataType& data, UniformType type) {
  std::stringstream ss;
  switch(type) {
#define UNIFORM_VARIABLE_TYPE(Type, Name)                                                          \
  case UniformType::Name:                                                                          \
    ss << boost::get<Type>(data);                                                                  \
    break;                                                                                         \
  case UniformType::VectorOf##Name:                                                                \
    ss << core::RangeToString(", ", "{", "}")(boost::get<std::vector<Type>>(data),                 \
                                              [](const Type& value) {                              \
                                                std::stringstream sout;                            \
                                                sout << value;                                     \
                                                return sout.str();                                 \
                                              });                                                  \
    break;
#include "sequoia-engine/Render/UniformVariable.inc"
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
    break;                                                                                         \
  case UniformType::VectorOf##Name:                                                                \
    os << BOOST_PP_STRINGIZE(VectorOf##Name);                                                      \
    break;
#include "sequoia-engine/Render/UniformVariable.inc"
#undef UNIFORM_VARIABLE_TYPE
  case UniformType::Invalid:
    os << "Invalid";
    break;
  case UniformType::Struct:
    os << "Struct";
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
    return boost::get<Type>(data_) == boost::get<Type>(other.data_);                               \
  case UniformType::VectorOf##Name:                                                                \
    return boost::get<std::vector<Type>>(data_) == boost::get<std::vector<Type>>(other.data_);
#include "sequoia-engine/Render/UniformVariable.inc"
#undef UNIFORM_VARIABLE_TYPE
  case UniformType::Invalid:
    return boost::get<internal::InvalidData>(data_) ==
           boost::get<internal::InvalidData>(other.data_);
  default:
    sequoia_unreachable("invalid type");
  }
}

std::string UniformVariable::toString() const {
  return core::format("UniformVariable[\n"
                      "  type = {},\n"
                      "  data = {}\n"
                      "]",
                      type_, core::indent(variantToString(data_, type_), 4));
}

std::ostream& operator<<(std::ostream& os, const UniformVariable& var) {
  return (os << var.toString());
}

} // namespace render

} // namespace sequoia
