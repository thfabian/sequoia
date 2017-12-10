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
struct SEQUOIA_API VertexLayout {
  constexpr VertexLayout() = default;

  /// @brief Type identifier
  enum TypeID : std::uint8_t {
    Invalid = 0,
#define VERTEX_LAYOUT_TYPE(Type, Name) Name,
#include "sequoia-engine/Render/VertexLayout.inc"
#undef VERTEX_LAYOUT_TYPE
  };

  /// @brief Vertex attribute (4 Bytes)
  ///
  /// Each attribute is described by an array of the form `T Attribute[N]`.
  struct Attribute {
    constexpr Attribute() = default;

    TypeID Type = Invalid;        ///< Type `T` of the attribute given as a `TypeID`
    std::uint8_t Offset = 0;      ///< Offset in the vertex struct (in bytes)
    std::uint8_t NumElements = 0; ///< Number of elements `N` of the attribute
    bool Normalize = false;       ///< Should the attribute be normalized when uploaded to the GPU?
  };

  /// @name Attribute description
  /// @{
  std::uint8_t SizeOf = 0;   ///< Total size (in bytes) of one vertex
  std::uint8_t VertexID = 0; ///< Identifier of the vertex `VertexID`
  Attribute Position;        ///< Positional attribute
  Attribute Normal;          ///< Normal attribute
  Attribute TexCoord;        ///< Texture coordinate attribute
  Attribute Color;           ///< Color attribute
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

template <VertexLayout::TypeID ID>
struct InvalidTypeIDToType {
  using type = void;
};

template <class T>
struct InvalidTypeToTypeID {
  static constexpr VertexLayout::TypeID value = VertexLayout::Invalid;
};

template <VertexLayout::TypeID ID>
struct TypeIDToType {
  using type = typename InvalidTypeIDToType<ID>::type;
  static_assert(!std::is_same<void, type>::value, "invalid VertexLayout::TypeID");
};

template <class T>
struct TypeToTypeID {
  static constexpr VertexLayout::TypeID value = InvalidTypeToTypeID<T>::value;
  static_assert(value != VertexLayout::Invalid, "invalid type (no match in VertexLayout::TypeID)");
};

#define VERTEX_LAYOUT_TYPE(Type, Name)                                                             \
  template <>                                                                                      \
  struct TypeIDToType<VertexLayout::Name> {                                                        \
    using type = Type;                                                                             \
  };                                                                                               \
  template <>                                                                                      \
  struct TypeToTypeID<Type> {                                                                      \
    static constexpr VertexLayout::TypeID value = VertexLayout::Name;                              \
  };
#include "sequoia-engine/Render/VertexLayout.inc"
#undef VERTEX_LAYOUT_TYPE

} // namespace internal

} // namespace render

} // namespace sequoia

#endif
