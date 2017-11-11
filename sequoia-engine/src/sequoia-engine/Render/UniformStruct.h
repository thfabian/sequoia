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

#ifndef SEQUOIA_ENGINE_RENDER_UNIFORMSTRUCT_H
#define SEQUOIA_ENGINE_RENDER_UNIFORMSTRUCT_H

#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Core/PreprocessorUtil.h"
#include "sequoia-engine/Render/UniformVariable.h"
#include <boost/preprocessor/tuple/elem.hpp>
#include <sstream>
#include <unordered_map>

/// @brief Get the `type` of a `(type, name)` tuple
#define SEQUOIA_PP_US_PARAM_GET_TYPE(Param) BOOST_PP_TUPLE_ELEM(2, 0, Param)

/// @brief Get the `name` of a `(type, name)` tuple
///@ingroup render
#define SEQUOIA_PP_US_PARAM_GET_NAME(Param) BOOST_PP_TUPLE_ELEM(2, 1, Param)

/// @brief Generate data members of a struct or class
///
/// From a `(type0, name0)...(typeN, nameN)` sequence, generate
///
/// @code{.cpp}
///
///  <type0> <name0>;
///  ...
///  <typeN> <nameN>;
///
/// @endcode
///@ingroup render
#define SEQUOIA_PP_US_GENERATE_MEMBERS(Members)                                                    \
  SEQUOIA_PP_SEQ_2_FOR_EACH(SEQUOIA_PP_US_GENERATE_MEMBER, Unused, Members)

#define SEQUOIA_PP_US_GENERATE_MEMBER(r, Data, Member)                                             \
  SEQUOIA_PP_US_PARAM_GET_TYPE(Member) SEQUOIA_PP_US_PARAM_GET_NAME(Member);

/// @brief Generate conversion to `UniformVariable`
///
/// From a `(type0, name0)...(typeN, nameN)` sequence, generate
///
/// @code{.cpp}
///
///   if(index == -1)
///     uniformMap[variableName + ".<name0>"] = UniformVariable(this-> <name0>);
///   else
///     uniformMap[core::format("%s[%i].<name0>", variableName, index]
///       = UniformVariable(this-> <name0>);
///
/// @endcode
///@ingroup render
#define SEQUOIA_PP_US_GENERATE_TO_UNIFORMVARIABLES(Members)                                        \
  SEQUOIA_PP_SEQ_2_FOR_EACH(SEQUOIA_PP_US_GENERATE_TO_UNIFORMVARIABLE, Unused, Members)

#define SEQUOIA_PP_US_GENERATE_TO_UNIFORMVARIABLE(r, Unused, Member)                               \
  if(index == -1)                                                                                  \
    uniformMap[variableName + "." BOOST_PP_STRINGIZE(SEQUOIA_PP_US_PARAM_GET_NAME(Member))] =      \
        ::sequoia::render::UniformVariable(this->SEQUOIA_PP_US_PARAM_GET_NAME(Member));            \
  else                                                                                             \
    uniformMap[ ::sequoia::core::format(                                                           \
        "{}[{}]." BOOST_PP_STRINGIZE(SEQUOIA_PP_US_PARAM_GET_NAME(Member)), variableName,          \
        index)] = ::sequoia::render::UniformVariable(this->SEQUOIA_PP_US_PARAM_GET_NAME(Member));

/// @brief Generate to string
///
/// From a `(type0, name0)...(typeN, nameN)` sequence, generate
///
/// @code{.cpp}
///
///   ss << "  <name0> = " << this-> <name0> << "\n";
///   ...
///   ss << "  <nameN> = " << this-> <nameN> << "\n";
///
/// @endcode
///@ingroup render
#define SEQUOIA_PP_US_GENERATE_TO_STRINGS(Members)                                                 \
  SEQUOIA_PP_SEQ_2_FOR_EACH(SEQUOIA_PP_US_GENERATE_TO_STRING, Unused, Members)

#define SEQUOIA_PP_US_GENERATE_TO_STRING(r, Data, Member)                                          \
  ss << "  " BOOST_PP_STRINGIZE(SEQUOIA_PP_US_PARAM_GET_NAME(Member)) " = "                        \
     << this->SEQUOIA_PP_US_PARAM_GET_NAME(Member) << "\n";

/// @brief Define a struct which matches an equivalent declaration in a GPU Program
///
/// For structs which are persistent accross all programs, use `SEQUOIA_UNIFORM_BLOCK`.
///
/// @param Name        Name of the generated struct
/// @param Members     A sequence of `(type0, name0)...(typeN, nameN)` pairs which declares the type
///                    and names of each of the struct members that are part of the sequence
///
/// @b Example: The following
///
/// @code{.cpp}
///   SEQUOIA_UNIFORM_STRUCT(MyStruct, (float, a)(math::vec3, b)(bool, c));
/// @endcode
///
/// results in a struct definition of
///
/// @code{.cpp}
///   struct MyStruct {
///     float a;
///     math::vec3 b;
///     bool c;
///   };
/// @endcode
///
/// @see sequoia::render::DrawCommand::setUniformStruct
///@ingroup render
#define SEQUOIA_UNIFORM_STRUCT(Name, Members)                                                      \
  struct Name {                                                                                    \
                                                                                                   \
    SEQUOIA_PP_US_GENERATE_MEMBERS(Members)                                                        \
                                                                                                   \
    inline void toUniformVariableMap(                                                              \
        const std::string& variableName,                                                           \
        std::unordered_map<std::string, ::sequoia::render::UniformVariable>& uniformMap,           \
        int index) const {                                                                         \
      SEQUOIA_PP_US_GENERATE_TO_UNIFORMVARIABLES(Members)                                          \
    }                                                                                              \
                                                                                                   \
    inline std::string toString() const {                                                          \
      std::stringstream ss;                                                                        \
      ss << BOOST_PP_STRINGIZE(Name) "[\n";                                                        \
      SEQUOIA_PP_US_GENERATE_TO_STRINGS(Members)                                                   \
      ss << "]";                                                                                   \
      return ss.str();                                                                             \
    }                                                                                              \
  }

#endif
