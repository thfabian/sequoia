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

#include "sequoia/Core/Exception.h"
#include "sequoia/Core/Format.h"
#include "sequoia/Core/Logging.h"
#include "sequoia/Core/Options2.h"
#include "sequoia/Core/StringSwitch.h"
#include "sequoia/Core/StringUtil.h"
#include "sequoia/Core/Unreachable.h"
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
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

template <class... Args>
bool isOneOf(const std::string& str, Args&&... args) {
  auto array = {args...};
  return std::find(std::begin(array), std::end(array), str) != std::end(array);
}

template <class From, class To>
struct ConvertImpl {
  static OptionsData apply(const OptionsData& dataFrom) {
    SEQUOIA_THROW(core::Exception, "cannot convert type '{}' to '{}'",
                  optionsTypeToString(internal::TypeToOptionsType<From>::value),
                  optionsTypeToString(internal::TypeToOptionsType<To>::value));
    return OptionsData{};
  }
};

// Bool
template <class To>
struct ConvertImpl<bool, To> {
  static OptionsData apply(const OptionsData& dataFrom) {
    return static_cast<To>(boost::get<bool>(dataFrom));
  }
};

template <>
struct ConvertImpl<bool, std::string> {
  static OptionsData apply(const OptionsData& dataFrom) {
    return std::to_string(boost::get<bool>(dataFrom));
  }
};

// Int
template <class To>
struct ConvertImpl<int, To> {
  static OptionsData apply(const OptionsData& dataFrom) {
    return static_cast<To>(boost::get<int>(dataFrom));
  }
};

template <>
struct ConvertImpl<int, std::string> {
  static OptionsData apply(const OptionsData& dataFrom) {
    return std::to_string(boost::get<int>(dataFrom));
  }
};

// Float
template <class To>
struct ConvertImpl<float, To> {
  static OptionsData apply(const OptionsData& dataFrom) {
    return static_cast<To>(boost::get<float>(dataFrom));
  }
};

template <>
struct ConvertImpl<float, std::string> {
  static OptionsData apply(const OptionsData& dataFrom) {
    return std::to_string(boost::get<float>(dataFrom));
  }
};

// String
template <>
struct ConvertImpl<std::string, bool> {
  static OptionsData apply(const OptionsData& dataFrom) {
    auto str = boost::get<std::string>(dataFrom);
    if(isOneOf(str, "1", "true", "True", "TRUE", "1.0"))
      return true;
    else if(isOneOf(str, "0", "false", "False", "FALSE", "0.0"))
      return true;
    else
      SEQUOIA_THROW(core::Exception, "cannot convert type '{}' (\"{}\") to '{}'",
                    optionsTypeToString(OptionsType::String), str,
                    optionsTypeToString(OptionsType::Bool));
  }
};

template <class To>
struct ConvertImpl<std::string, To> {
  static OptionsData apply(const OptionsData& dataFrom) {
    auto str = boost::get<std::string>(dataFrom);
    try {
      return boost::lexical_cast<To>(str);
    } catch(const boost::bad_lexical_cast& e) {
      SEQUOIA_THROW(core::Exception, "cannot convert type '{}' (\"{}\") to '{}' : {}",
                    optionsTypeToString(OptionsType::String), str,
                    optionsTypeToString(internal::TypeToOptionsType<To>::value), e.what());
    }
    return OptionsData{};
  }
};

template <>
struct ConvertImpl<std::string, std::string> {
  static OptionsData apply(const OptionsData& dataFrom) {
    return boost::get<std::string>(dataFrom);
  }
};

OptionsData convert(OptionsType typeTo, OptionsType typeFrom, const OptionsData& dataFrom) {
  switch(typeFrom) {
  case OptionsType::Bool: {
    switch(typeTo) {
#define OPTIONS_TYPE(Type, Name)                                                                   \
  case OptionsType::Name:                                                                          \
    return ConvertImpl<bool, Type>::apply(dataFrom);
#include "sequoia/Core/OptionsType.inc"
#undef OPTIONS_TYPE
    default:
      SEQUOIA_THROW(core::Exception, "cannot convert type '{}' to '{}'",
                    optionsTypeToString(typeFrom), optionsTypeToString(typeFrom));
    }
  }

  case OptionsType::Int: {
    switch(typeTo) {
#define OPTIONS_TYPE(Type, Name)                                                                   \
  case OptionsType::Name:                                                                          \
    return ConvertImpl<int, Type>::apply(dataFrom);
#include "sequoia/Core/OptionsType.inc"
#undef OPTIONS_TYPE
    default:
      SEQUOIA_THROW(core::Exception, "cannot convert type '{}' to '{}'",
                    optionsTypeToString(typeFrom), optionsTypeToString(typeFrom));
    }
  }

  case OptionsType::Float: {
    switch(typeTo) {
#define OPTIONS_TYPE(Type, Name)                                                                   \
  case OptionsType::Name:                                                                          \
    return ConvertImpl<float, Type>::apply(dataFrom);
#include "sequoia/Core/OptionsType.inc"
#undef OPTIONS_TYPE
    default:
      SEQUOIA_THROW(core::Exception, "cannot convert type '{}' to '{}'",
                    optionsTypeToString(typeFrom), optionsTypeToString(typeFrom));
    }
  }

  case OptionsType::String: {
    switch(typeTo) {
#define OPTIONS_TYPE(Type, Name)                                                                   \
  case OptionsType::Name:                                                                          \
    return ConvertImpl<std::string, Type>::apply(dataFrom);
#include "sequoia/Core/OptionsType.inc"
#undef OPTIONS_TYPE
    default:
      SEQUOIA_THROW(core::Exception, "cannot convert type '{}' to '{}'",
                    optionsTypeToString(typeFrom), optionsTypeToString(typeFrom));
    }
  }

  default:
    SEQUOIA_THROW(core::Exception, "cannot convert type '{}' to '{}'",
                  optionsTypeToString(typeFrom), optionsTypeToString(typeFrom));
  }
  sequoia_unreachable("invalid conversion");
}

} // namespace internal

std::string Option::toString() const {
  return core::format("Option[\n"
                      "  type = {}\n"
                      "  data = {}\n"
                      "]",
                      optionsTypeToString(type_), variantToString(data_, type_));
}

std::string OptionMetaData::toString() const {
  return core::format("OptionMetaData[\n"
                      "  CommandLine = {}\n"
                      "  CommandLineShort = {}\n"
                      "  CommandLineMetaVar = {}\n"
                      "  DocString = {}\n"
                      "]",
                      CommandLine, CommandLineShort, CommandLineMetaVar, DocString);
}

const Option& Options2::getAsOption(const std::string& name) const {
  auto it = options_.find(name);
  if(it == options_.end())
    SEQUOIA_THROW(core::Exception, "option '{}' does not exists", name);
  return it->second;
}

void Options2::write(const platform::Path& file) const {
  auto path = platform::toAnsiString(file);
  LOG(INFO) << "Writing Options to \"" << path << "\" ...";

  boost::property_tree::ptree ptree;
  for(const auto& option : options_)
    ptree.put(option.first, option.second.template get<std::string>());

  try {
    boost::property_tree::write_ini(path, ptree);
  } catch(boost::property_tree::ini_parser_error& e) {
    LOG(WARNING) << "Failed to write Options : " << e.what();
    SEQUOIA_THROW(core::Exception, "failed to write Options : {}", e.what());
  }

  LOG(INFO) << "Successfully wrote Options to \"" << path << "\"";
}

void Options2::read(const platform::Path& file) {}

Option& Options2::getAsOption(const std::string& name) {
  auto it = options_.find(name);
  if(it == options_.end())
    SEQUOIA_THROW(core::Exception, "option '{}' does not exists", name);
  return it->second;
}

std::string Options2::toString() const {
  return core::format(
      "Options[\n"
      "  options = {},\n"
      "  optionsMetaData = {}\n"
      "]",
      core::indent(core::toStringRange(options_,
                                       [](const std::pair<std::string, Option>& optionPair) {
                                         return core::format(
                                             "{} = {}", optionPair.first,
                                             core::indent(optionPair.second.toString()));
                                       })),
      optionsMetaData_.empty()
          ? "null"
          : core::indent(core::toStringRange(
                optionsMetaData_, [](const std::pair<std::string, OptionMetaData>& optionPair) {
                  return core::format("{} = {}", optionPair.first,
                                      core::indent(optionPair.second.toString()));
                })));
}

} // namespace core

} // namespace sequoia
