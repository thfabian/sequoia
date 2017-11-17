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

#ifndef SEQUOIA_ENGINE_RENDER_VERTEXDEF_H
#define SEQUOIA_ENGINE_RENDER_VERTEXDEF_H

#include "sequoia-engine/Core/PreprocessorUtil.h"
#include "sequoia-engine/Render/VertexLayout.h"
#include <boost/preprocessor/comma.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/empty.hpp>
#include <boost/preprocessor/if.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>
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
#define SEQUOIA_DEFINE_VERTEX(Name, Attributes)                                                    \
  struct Name {                                                                                    \
    SEQUOIA_PP_VERTEX_GENERATE_MEMBERS(Attributes)                                                 \
                                                                                                   \
    constexpr static VertexLayout2 getLayout() noexcept {                                          \
      VertexLayout2 layout;                                                                        \
      SEQUOIA_PP_VERTEX_GENERATE_LAYOUTS(Attributes, Name)                                         \
      layout.SizeOf = sizeof(Name);                                                                \
      layout.VertexID = static_cast<decltype(layout.VertexID)>(VertexID::Name);                    \
      return layout;                                                                               \
    }                                                                                              \
  }

/// @brief Generate a sequence of all registered vertices
/// @ingroup render
#define SEQUOIA_REGSTER_VERTICES(...) BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)

/// @brief Create an enum from `data`
/// @ingroup render
#define SEQUOIA_PP_ENUM_ELEMENT(r, Data, Elem) Elem,

/// @brief Create a list of comma separated enum from the `VertexList` sequence
/// @ingroup render
#define SEQUOIA_DEFINE_VERTEX_ID_ENUM(VertexList)                                                  \
  enum class VertexID : std::uint8_t {                                                             \
    Invalid = 0,                                                                                   \
    BOOST_PP_SEQ_FOR_EACH(SEQUOIA_PP_ENUM_ELEMENT, Data, VertexList)                               \
  };

#endif
