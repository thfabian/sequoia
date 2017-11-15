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

// -- VertexLayout.h

#ifndef SEQUOIA_ENGINE_RENDER_VERTEXLAYOUT_H
#define SEQUOIA_ENGINE_RENDER_VERTEXLAYOUT_H

#include "sequoia-engine/Core/Assert.h"
#include "sequoia-engine/Core/Export.h"
#include <cstdint>
#include <string>

namespace sequoia {

namespace render {

/// @brief Layout description of vertices
/// @ingroup render
struct SEQUOIA_API VertexLayout2 {

  /// @brief Type identifier
  enum TypeID : std::uint8_t {
    Invalid = 0,
    UInt8,  ///< 8-bit unsigned integer
    Float32 ///< 32-bit floating point number
  };

  /// @brief Vertex attribute (4 Bytes)
  /// 
  /// Each attribute is described by an array of the form `T Attribute[N]`.
  struct Attribute {
    TypeID Type = Invalid;        ///< Type `T` of the attribute given as a `TypeID`
    std::uint8_t Offset = 0;      ///< Offset in the vertex struct (in bytes)
    std::uint8_t NumElements = 0; ///< Number of elements `N` of the attribute
    bool Normalize = false;       ///< Should the attribute be normalized when uploaded to the GPU?
  };

  /// @name Attribute description
  /// @{
  std::int32_t SizeOf = 0; ///< Total size (in bytes) of one vertex
  Attribute Position;      ///< Positional attribute
  Attribute Normal;        ///< Normal attribute
  Attribute TexCoord;      ///< Texture coordinate attribute
  Attribute Color;         ///< Color attribute
  /// @}

  /// @brief Check if `Position` attribute is available
  bool hasPosition() const { return Position.NumElements > 0; }

  /// @brief Check if `Normal` attribute is available
  bool hasNormal() const { return Normal.NumElements > 0; }

  /// @brief Check if `TexCoord` attribute is available
  bool hasTexCoord() const { return TexCoord.NumElements > 0; }

  /// @brief Check if `Color` attribute is available
  bool hasColor() const { return Color.NumElements > 0; }

  /// @brief Convert to string
  std::string toString() const;
};

namespace internal {

template <VertexLayout2::TypeID ID>
struct InvalidTypeIDToType {
  using type = void;
};

template <class T>
struct InvalidTypeToTypeID {
  static constexpr VertexLayout2::TypeID value = VertexLayout2::Invalid;
};

template <VertexLayout2::TypeID ID>
struct TypeIDToType {
  using type = typename InvalidTypeIDToType<ID>::type;
  static_assert(!std::is_same<void, type>::value, "invalid VertexLayout::TypeID");
};

template <class T>
struct TypeToTypeID {
  static constexpr VertexLayout2::TypeID value = InvalidTypeToTypeID<T>::value;
  static_assert(value != VertexLayout2::Invalid, "invalid type (no match in VertexLayout::TypeID)");
};

// std::uint8_t
template <>
struct TypeIDToType<VertexLayout2::UInt8> {
  using type = std::uint8_t;
};

template <>
struct TypeToTypeID<std::uint8_t> {
  static constexpr VertexLayout2::TypeID value = VertexLayout2::UInt8;
};

// float
template <>
struct TypeIDToType<VertexLayout2::Float32> {
  using type = float;
};

template <>
struct TypeToTypeID<float> {
  static constexpr VertexLayout2::TypeID value = VertexLayout2::Float32;
};

} // namespace internal

} // namespace render

} // namespace sequoia

#endif

#ifndef SEQUOIA_ENGINE_RENDER_VERTEXDEF_H
#define SEQUOIA_ENGINE_RENDER_VERTEXDEF_H

// -- VertexDef.h

#include "sequoia-engine/Core/PreprocessorUtil.h"
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/empty.hpp>
#include <boost/preprocessor/if.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <cstddef>

/// @brief Get the `type` of a `(type, attribute, rank, normalize)` tuple
/// @ingroup render
#define SEQUOIA_PP_VERTEX_PARAM_GET_TYPE(Param) BOOST_PP_TUPLE_ELEM(4, 0, Param)

/// @brief Get the `attribute` of a `(type, attribute, rank, normalize)` tuple
/// @ingroup render
#define SEQUOIA_PP_VERTEX_PARAM_GET_ATTRIBUTE(Param) BOOST_PP_TUPLE_ELEM(4, 1, Param)

/// @brief Get the `rank` of a `(type, attribute, rank, normalize)` tuple
/// @ingroup render
#define SEQUOIA_PP_VERTEX_PARAM_GET_RANK(Param) BOOST_PP_TUPLE_ELEM(4, 2, Param)

/// @brief Get the `normalize` of a `(type, attribute, rank, normalize)` tuple
/// @ingroup render
#define SEQUOIA_PP_VERTEX_PARAM_GET_NORMALIZE(Param) BOOST_PP_TUPLE_ELEM(4, 3, Param)

/// @brief Generate data members of of the vertex
///
/// From a `(type0, attribute0, rank0, normalize0)...(typeN, attributeN, rankN, normalize0)`
/// sequence, generate
///
/// @code{.cpp}
///
///  <type0> <attribute0> [<rank0>];
///  ...
///  <typeN> <attributeN> [<rankN>];
///
/// @endcode
/// @ingroup render
#define SEQUOIA_PP_VERTEX_GENERATE_MEMBERS(Members)                                                \
  SEQUOIA_PP_SEQ_4_FOR_EACH(SEQUOIA_PP_VERTEX_GENERATE_MEMBER, Unused, Members)

#define SEQUOIA_PP_VERTEX_GENERATE_MEMBER(r, Data, Member)                                         \
  SEQUOIA_PP_VERTEX_PARAM_GET_TYPE(Member)                                                         \
  SEQUOIA_PP_VERTEX_PARAM_GET_ATTRIBUTE(Member)                                                    \
  BOOST_PP_IF(BOOST_PP_EQUAL(SEQUOIA_PP_VERTEX_PARAM_GET_RANK(Member), 1), BOOST_PP_EMPTY(),       \
              [SEQUOIA_PP_VERTEX_PARAM_GET_RANK(Member)]);

/// @brief Populate the layout of the vertex
///
/// From a `(type0, attribute0, rank0, normalize0)...(typeN, attributeN, rankN, normalize0)`
/// sequence, generate
///
/// @code{.cpp}
///
///   layout.<attribute0>.Type = ::sequoia::render::internal::TypeToTypeID<<type0>>::value;
///   layout.<attribute0>.Offset = offsetof(<Name>, <attribute0>);
///   layout.<attribute0>.NumElements = <rank0>;
///   layout.<attribute0>.Normalize = <normalize0>;
///   ...
///
/// @endcode
/// @ingroup render
#define SEQUOIA_PP_VERTEX_GENERATE_LAYOUTS(Members, Name)                                          \
  SEQUOIA_PP_SEQ_4_FOR_EACH(SEQUOIA_PP_VERTEX_GENERATE_LAYOUT, Name, Members)

#define SEQUOIA_PP_VERTEX_GENERATE_LAYOUT(r, Data, Member)                                         \
  layout.SEQUOIA_PP_VERTEX_PARAM_GET_ATTRIBUTE(Member).Type =                                      \
      ::sequoia::render::internal::TypeToTypeID<SEQUOIA_PP_VERTEX_PARAM_GET_TYPE(Member)>::value;  \
  layout.SEQUOIA_PP_VERTEX_PARAM_GET_ATTRIBUTE(Member).Offset =                                    \
      offsetof(Data, SEQUOIA_PP_VERTEX_PARAM_GET_ATTRIBUTE(Member));                               \
  layout.SEQUOIA_PP_VERTEX_PARAM_GET_ATTRIBUTE(Member).NumElements =                               \
      SEQUOIA_PP_VERTEX_PARAM_GET_RANK(Member);                                                    \
  layout.SEQUOIA_PP_VERTEX_PARAM_GET_ATTRIBUTE(Member).Normalize =                                 \
      SEQUOIA_PP_VERTEX_PARAM_GET_NORMALIZE(Member);

/// @brief Generate the vertex `Name` description
///
/// @param Name         Name of the vertex i.e struct
/// @param Attributes   Tuple of (type, attribute, rank, normalize) where:
///                       - type      : Is the C++ type of the attribute (e.g `float`)
///                       - attribute : The name of the attribute `(Position, Normal, TexCoord,
///                                     Color)`
///                       - rank      : Rank or array size of the attribute
///                       - normalize : Boolean which indicates if the attribute needs to be
///                                     normalized to `[0, 1]` when uploading it to the GPU
/// 
/// @b Example
/// 
/// The following
///
/// @code{.cpp}
///   SEQUOIA_VERTEX(TestVertex,
///                 (float, Position, 3, false)
///                 (float, Normal, 3, false)
///                 (float, TexCoord, 2, false)
///                 (std::uint8_t, Color, 4, true));
/// @endcode
///
/// generates a struct
///
/// @code{.cpp}
/// struct TestVertex {
///   float Position[3];
///   float Normal[3];
///   float TexCoord[2];
///   std::uint8_t Color[4];
///
///   VertexLayout getLayout() noexcept {
///     ...
///   }
/// };
/// @endcode
///
/// where the code for populating the `VertexLayout` in `getLayout()` will be generated 
/// automatically.
///
/// @ingroup render
#define SEQUOIA_VERTEX(Name, Attributes)                                                           \
  struct Name {                                                                                    \
    SEQUOIA_PP_VERTEX_GENERATE_MEMBERS(Attributes)                                                 \
                                                                                                   \
    inline static VertexLayout2 getLayout() noexcept {                                             \
      VertexLayout2 layout;                                                                        \
      SEQUOIA_PP_VERTEX_GENERATE_LAYOUTS(Attributes, Name)                                         \
      layout.SizeOf = sizeof(Name);                                                                \
      return layout;                                                                               \
    }                                                                                              \
  }

#endif

// ---
