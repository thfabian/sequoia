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

#ifndef SEQUOIA_ENGINE_RENDER_UNIFORMBLOCK_H
#define SEQUOIA_ENGINE_RENDER_UNIFORMBLOCK_H

#include "sequoia-engine/Core/PreprocessorUtil.h"
#include "sequoia-engine/Core/StringUtil.h"
#include "sequoia-engine/Render/UniformVariable.h"
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/empty.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <sstream>
#include <vector>

/// @brief Get the `type` of a `(type, name, rank)` tuple
/// @ingroup render
#define SEQUOIA_PP_UB_PARAM_GET_TYPE(Param) BOOST_PP_TUPLE_ELEM(3, 0, Param)

/// @brief Get the `name` of a `(type, name, rank)` tuple
/// @ingroup render
#define SEQUOIA_PP_UB_PARAM_GET_NAME(Param) BOOST_PP_TUPLE_ELEM(3, 1, Param)

/// @brief Get the `rank` of a `(type, name, rank)` tuple
/// @ingroup render
#define SEQUOIA_PP_UB_PARAM_GET_RANK(Param) BOOST_PP_TUPLE_ELEM(3, 2, Param)

/// @brief Generate data members of a struct or class
///
/// From a `(type0, name0, rank0)...(typeN, nameN, rankN)` sequence, generate
///
/// @code{.cpp}
///
///  <type0> <name0> [<rank1>];
///  ...
///  <typeN> <nameN> [<rank1>];
///
/// @endcode
///
/// Note that <rank> is dropped if it is equal to 1.
///
/// @ingroup render
#define SEQUOIA_PP_UB_GENERATE_MEMBERS(Members)                                                    \
  SEQUOIA_PP_SEQ_3_FOR_EACH(SEQUOIA_PP_UB_GENERATE_MEMBER, Unused, Members)

#define SEQUOIA_PP_UB_GENERATE_MEMBER(r, Data, Member)                                             \
  SEQUOIA_PP_UB_PARAM_GET_TYPE(Member)                                                             \
  SEQUOIA_PP_UB_PARAM_GET_NAME(Member)                                                             \
  BOOST_PP_IF(BOOST_PP_EQUAL(SEQUOIA_PP_UB_PARAM_GET_RANK(Member), 1), BOOST_PP_EMPTY(),           \
              [SEQUOIA_PP_UB_PARAM_GET_RANK(Member)]);

/// @brief Generate to string
///
/// From a `(type0, name0, rank0)...(typeN, nameN, rankN)` sequence, generate
///
/// @code{.cpp}
///
///   ss << "  <name0> = " << this-> <name0> << "\n";
///   ...
///   ss << "  <nameN> = " << this-> <nameN> << "\n";
///
/// @endcode
/// @ingroup render
#define SEQUOIA_PP_UB_GENERATE_TO_STRINGS(Members)                                                 \
  SEQUOIA_PP_SEQ_3_FOR_EACH(SEQUOIA_PP_UB_GENERATE_TO_STRING, Unused, Members)

#define SEQUOIA_PP_UB_GENERATE_TO_STRING(r, Data, Member)                                          \
  ss << "  " BOOST_PP_STRINGIZE(SEQUOIA_PP_UB_PARAM_GET_NAME(Member)) " = "                        \
     << ::sequoia::render::internal::toString(this->SEQUOIA_PP_UB_PARAM_GET_NAME(Member)) << "\n";

/// @brief Populate `UniformBlockMemberInfo` of each member
///
/// From a `(type0, name0, rank0)...(typeN, nameN, rankN)` sequence, generate
///
/// @endcode
/// @ingroup render
#define SEQUOIA_PP_UB_GENERATE_MEMBERINFOS(Members, Name)                                          \
  SEQUOIA_PP_SEQ_3_FOR_EACH(SEQUOIA_PP_UB_GENERATE_MEMBERINFO, Name, Members)

#define SEQUOIA_PP_UB_GENERATE_MEMBERINFO(r, Data, Member)                                         \
  memberInfo.emplace_back(::sequoia::render::UniformBlockMemberInfo{                               \
      BOOST_PP_STRINGIZE(SEQUOIA_PP_UB_PARAM_GET_NAME(Member)),                                    \
      ::sequoia::render::internal::GetUniformBlockType<SEQUOIA_PP_UB_PARAM_GET_TYPE(               \
          Member)>::value,                                                                         \
      offsetof(Data, SEQUOIA_PP_UB_PARAM_GET_NAME(Member)),                                        \
      sizeof(SEQUOIA_PP_UB_PARAM_GET_TYPE(Member)) * SEQUOIA_PP_UB_PARAM_GET_RANK(Member),         \
      SEQUOIA_PP_UB_PARAM_GET_RANK(Member)});

namespace sequoia {

namespace render {

namespace internal {

template <bool IsStruct, class T>
struct GetUniformBlockTypeImpl {
  static constexpr UniformType value = internal::TypeToUniformType<T>::value;
};

template <class T>
struct GetUniformBlockTypeImpl<true, T> {
  static_assert(std::is_class<T>::value, "member of UniformBlock is not a class/struct type");
  static_assert(std::is_pod<T>::value, "member of UniformBlock is not a POD type");
  static constexpr UniformType value = UniformType::Struct;
};

template <class T>
struct GetUniformBlockType {
  static constexpr UniformType value =
      GetUniformBlockTypeImpl<!internal::IsUniformType<T>::value, T>::value;
};

template <bool IsStruct, class T>
struct ToStringImpl {
  std::string operator()(const T& value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
  }
};

template <class T>
struct ToStringImpl<true, T> {
  std::string operator()(const T& value) { return value.toString(); }
};

template <bool IsArray, class T>
struct ToStringArrayImpl {
  std::string operator()(const T& value) {
    return ToStringImpl<!internal::IsUniformType<T>::value, T>()(value);
  }
};

template <class T>
struct ToStringArrayImpl<true, T> {
  std::string operator()(const T& value) {
    return ::sequoia::core::indent(::sequoia::core::toStringRange(value, [](const auto& v) {
      using ValueType = typename std::remove_all_extents<T>::type;
      return ::sequoia::core::indent(
          ToStringImpl<!internal::IsUniformType<ValueType>::value, ValueType>()(v));
    }));
  }
};

template <class T>
inline std::string toString(const T& value) {
  return ToStringArrayImpl<std::is_array<T>::value, T>()(value);
}

} // namespace internal

/// @brief Detailed information about each member in the UniformBlock
/// @ingroup render
struct UniformBlockMemberInfo {
  const char* Name;     ///< Name of the the member
  UniformType Type;     ///< Type identifier of the member
  std::uint32_t Offset; ///< Byte offset of the member
  std::uint32_t Size;   ///< Total size (in Bytes) of the member
  std::uint32_t Rank;   ///< Rank of the member (size of array)
};

} // namespace game

} // namespace sequoia

#define SEQUOIA_UNIFORM_BLOCK(Name, Members)                                                       \
  class Name {                                                                                     \
  public:                                                                                          \
    SEQUOIA_PP_UB_GENERATE_MEMBERS(Members)                                                        \
                                                                                                   \
    inline std::string toString() const {                                                          \
      std::stringstream ss;                                                                        \
      ss << BOOST_PP_STRINGIZE(Name) "[\n";                                                        \
      SEQUOIA_PP_UB_GENERATE_TO_STRINGS(Members) ss << "]";                                        \
      return ss.str();                                                                             \
    }                                                                                              \
                                                                                                   \
    static inline std::vector<::sequoia::render::UniformBlockMemberInfo> GetMemberInfo() {         \
      std::vector<::sequoia::render::UniformBlockMemberInfo> memberInfo;                           \
      SEQUOIA_PP_UB_GENERATE_MEMBERINFOS(Members, Name)                                            \
      return memberInfo;                                                                           \
    }                                                                                              \
  }

#endif
