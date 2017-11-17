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
struct SEQUOIA_API VertexLayout2 {
  constexpr VertexLayout2() = default;

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
