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
#include "sequoia/Core/Options2.h"
#include "sequoia/Core/StringUtil.h"
#include "sequoia/Core/Unreachable.h"
#include <ostream>
#include <sstream>

namespace sequoia {

namespace core {

namespace {

static std::string variantToString(const internal::OptionsData& data, internal::OptionsType type) {
  std::stringstream ss;
  switch(type) {
#define OPTIONS_TYPE(Type, Name)                                                                   \
  case internal::OptionsType::Name:                                                                \
    ss << boost::get<Type>(data);                                                                  \
    break;
#include "sequoia/Core/OptionsType.inc"
#undef OPTIONS_TYPE
  case internal::OptionsType::Invalid:
    ss << "<invalid>";
    break;
  default:
    sequoia_unreachable("invalid type");
  }
  return ss.str();
}

static const char* optionsTypeToString(internal::OptionsType type) {
  switch(type) {
#define OPTIONS_TYPE(Type, Name)                                                                   \
  case internal::OptionsType::Name:                                                                \
    return #Name;
#include "sequoia/Core/OptionsType.inc"
#undef OPTIONS_TYPE
  case internal::OptionsType::Invalid:
    return "<invalid>";
  default:
    sequoia_unreachable("invalid type");
  }
}

} // anonymous namespace

namespace internal {

OptionsData convertTo(OptionsType typeTo, OptionsType typeFrom, const OptionsData& dataFrom) {
  return OptionsData{};
}

} // namespace internal

std::string Option::toString() const {
  return core::format("Option[\n"
                      "  type = {}\n"
                      "  data = {}\n"
                      "]",
                      optionsTypeToString(type_), variantToString(data_, type_));
}

std::string Options2::toString() const {
  return core::format(
      "Options[\n"
      "  options = {}\n"
      "]",
      core::toStringRange(options_, [](const std::pair<std::string, Option>& optionPair) {
        return core::format("{} = {}", optionPair.first, optionPair.second.toString());
      }));
}

} // namespace core

} // namespace sequoia
