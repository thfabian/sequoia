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

#ifndef SEQUOIA_RENDER_UNIFORMSTRUCT_H
#define SEQUOIA_RENDER_UNIFORMSTRUCT_H

#include "sequoia/Core/Export.h"
#include "sequoia/Core/Format.h"
#include "sequoia/Render/UniformVariable.h"
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <sstream>
#include <unordered_map>

#define SEQUOIA_SEQ_FILLER_0(X, Y) ((X, Y)) SEQUOIA_SEQ_FILLER_1
#define SEQUOIA_SEQ_FILLER_1(X, Y) ((X, Y)) SEQUOIA_SEQ_FILLER_0
#define SEQUOIA_SEQ_FILLER_0_END
#define SEQUOIA_SEQ_FILLER_1_END

// Instead of using `Sequence` directly, we use `BOOST_PP_CAT(SEQUOIA_SEQ_FILLER_0 Sequence, _END)`.
// This is a clever trick to force the double parentheses. It works like this (assuming Sequence is
// `(int, x)(float, y)`:
//
// - Start with: `SEQUOIA_SEQ_FILLER_0(int,x)(float,y)_END`
// - Expand `SEQUOIA_SEQ_FILLER_0(A,B)` to `((A,B))SEQUOIA_SEQ_FILLER_1`
//   yielding `((int,x))SEQUOIA_SEQ_FILLER_1(float,y)_END`
// - Expand `SEQUOIA_SEQ_FILLER_1(A,B)` to `((A,B))SEQUOIA_SEQ_FILLER_0`
//   yielding `((int,x))((float,y))CREATE_MY_MACRO_PLACEHOLDER_FILLER_0_END`
// - Expand `SEQUOIA_SEQ_FILLER_0_END` to ``
//   yielding `((int,x))((float,y))`
//
// This is inspired by BOOST_FUSION_ADAPT_STRUCT and friends.
#define SEQUOIA_GENERATE_MACRO(Macro, Data, Sequence)                                              \
  BOOST_PP_SEQ_FOR_EACH(Macro, Data, BOOST_PP_CAT(SEQUOIA_SEQ_FILLER_0 Sequence, _END))

// Get the `type` of a `(type, name)` tuple
#define SEQUOIA_PARAM_GET_TYPE(Param) BOOST_PP_TUPLE_ELEM(2, 0, Param)

// Get the `name` of a `(type, name)` tuple
#define SEQUOIA_PARAM_GET_NAME(Param) BOOST_PP_TUPLE_ELEM(2, 1, Param)

// Generate
//
//  <type> <name>;
//
// from a `(type0, name0)...(typeN, nameN)` sequence
#define SEQUOIA_GENERATE_MEMBER(r, Data, Member)                                                   \
  SEQUOIA_PARAM_GET_TYPE(Member) SEQUOIA_PARAM_GET_NAME(Member);

#define SEQUOIA_GENERATE_MEMBERS(Members)                                                          \
  SEQUOIA_GENERATE_MACRO(SEQUOIA_GENERATE_MEMBER, Unused, Members)

// Generate
//
//   if(index == -1)
//     uniformMap["<variableName>.<name>"] = UniformVariable(this-><name>);
//   else
//     uniformMap[core::format("<variableName>.<name>[%i]", index] = UniformVariable(this-><name>);
//
// from a `(type0, name0)...(typeN, nameN)` sequence
#define SEQUOIA_GENERATE_TO_UNIFORMVARIABLE(r, Unused, Member)                                     \
  if(index == -1)                                                                                  \
    uniformMap[variableName + "." BOOST_PP_STRINGIZE(SEQUOIA_PARAM_GET_NAME(Member))] =            \
        ::sequoia::render::UniformVariable(this->SEQUOIA_PARAM_GET_NAME(Member));                  \
  else                                                                                             \
    uniformMap[core::format("%s[%i]." BOOST_PP_STRINGIZE(SEQUOIA_PARAM_GET_NAME(Member)),          \
                            variableName, index)] =                                                \
        ::sequoia::render::UniformVariable(this->SEQUOIA_PARAM_GET_NAME(Member));

#define SEQUOIA_GENERATE_TO_UNIFORMVARIABLES(Members)                                              \
  SEQUOIA_GENERATE_MACRO(SEQUOIA_GENERATE_TO_UNIFORMVARIABLE, Unused, Members)

// Generate
//
//   ss << "  <name> = " << this-><name> << "\n";
//
// from a `(type0, name0)...(typeN, nameN)` sequence
#define SEQUOIA_GENERATE_TO_STRING(r, Data, Member)                                                \
  ss << "  " BOOST_PP_STRINGIZE(SEQUOIA_PARAM_GET_NAME(Member)) " = "                              \
     << this->SEQUOIA_PARAM_GET_NAME(Member) << "\n";

#define SEQUOIA_GENERATE_TO_STRINGS(Members)                                                       \
  SEQUOIA_GENERATE_MACRO(SEQUOIA_GENERATE_TO_STRING, Unused, Members)

/// @brief Define a struct which matches an equivalent declaration in a GPU Program
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
/// @ingroup render
#define SEQUOIA_UNIFORM_STRUCT(Name, Members)                                                      \
  struct Name {                                                                                    \
    SEQUOIA_GENERATE_MEMBERS(Members)                                                              \
    inline void toUniformVariableMap(                                                              \
        const std::string& variableName,                                                           \
        std::unordered_map<std::string, ::sequoia::render::UniformVariable>& uniformMap,           \
        int index) const {                                                                         \
      SEQUOIA_GENERATE_TO_UNIFORMVARIABLES(Members)                                                \
    }                                                                                              \
    inline std::string toString() const {                                                          \
      std::stringstream ss;                                                                        \
      ss << BOOST_PP_STRINGIZE(Name) "[\n";                                                        \
      SEQUOIA_GENERATE_TO_STRINGS(Members)                                                         \
      ss << "]";                                                                                   \
      return ss.str();                                                                             \
    }                                                                                              \
  }

#endif
