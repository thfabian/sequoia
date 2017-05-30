//===--------------------------------------------------------------------------------*- C++ -*-===//
//                         _____                        _
//                        / ____|                      (_)
//                       | (___   ___  __ _ _   _  ___  _  __ _
//                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
//                        ____) |  __/ (_| | |_| | (_) | | (_| |
//                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine
//                                       | |
//                                       |_|
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#ifndef SEQUOIA_RENDER_VERTEX_H
#define SEQUOIA_RENDER_VERTEX_H

#include "sequoia/Core/Assert.h"
#include "sequoia/Render/Export.h"
#include <cstddef>
#include <string>

namespace sequoia {

namespace render {

struct VertexLayout;
struct Vertex2DLayout;
struct Vertex3DLayout;
class VertexVisitor;

/// @addtogroup render
/// @{

/// @typedef VertexIndicesType
/// @brief Type of vertex indices
using VertexIndexType = unsigned int;

//===------------------------------------------------------------------------------------------===//
//    VertexLayout
//===------------------------------------------------------------------------------------------===//

/// @brief Layout description of vertices
///
/// Each attribute is described by an array of the form `T Attribute[N]` in the vertex struct with:
///   - @b XType : Type `T` of the attribute (e.g `float`)
///   - @b XOffset : Offset in the vertex struct (in bytes)
///   - @b XNumElement : Number of elements `N` of the attribute
///   - @b XNormalized : Should be attribute be normalized when uploaded to the GPU?
///
/// where @b X is one of `{ Position, Normal, TexCoord, Color}`.
struct SEQUOIA_RENDER_API VertexLayout {

  enum Type {
    Invalid,
    UnsignedByte, ///< 8-bit byte
    Float         ///< 32-bit floating point number
  };

  std::size_t SizeOf = 0; ///< Total size (in bytes) of one vertex

  Type PositionType = Invalid;
  std::size_t PositionOffset = 0;
  std::size_t PositionNumElement = 0;
  bool PositionNormalized = false;

  Type NormalType = Invalid;
  std::size_t NormalOffset = 0;
  std::size_t NormalNumElement = 0;
  bool NormalNormalized = false;

  Type TexCoordType = Invalid;
  std::size_t TexCoordOffset = 0;
  std::size_t TexCoordNumElement = 0;
  bool TexCoordNormalized = false;

  Type ColorType = Invalid;
  std::size_t ColorOffset = 0;
  std::size_t ColorNumElement = 0;
  bool ColorNormalized = false;

  /// @brief Check if `Position` attribute is available
  bool hasPosition() const { return PositionNumElement > 0; }

  /// @brief Check if `Normal` attribute is available
  bool hasNormal() const { return NormalNumElement > 0; }

  /// @brief Check if `TexCoord` attribute is available
  bool hasTexCoord() const { return TexCoordNumElement > 0; }

  /// @brief Check if `Color` attribute is available
  bool hasColor() const { return ColorNumElement > 0; }

  /// @brief Accept a VertexVisitor
  virtual void accept(VertexVisitor& visitor) const = 0;

  /// @brief Convert to string
  std::string toString() const;
};

//===------------------------------------------------------------------------------------------===//
//    Vertex2D
//===------------------------------------------------------------------------------------------===//

/// @brief Representation of a 2D vertex
struct SEQUOIA_RENDER_API Vertex2D {
  using PositionType = float;
  using TexCoordType = float;
  using ColorType = unsigned char;

  PositionType Position[2];
  TexCoordType TexCoord[2];
  ColorType Color[4];

  /// @brief Get the layout of the 2D vertices
  /// @note This function is thread-safe.
  static const VertexLayout* getLayout() noexcept;

  /// @brief Convert to string
  static std::string toString(const Vertex2D& vertex);
};

/// @brief Layout description of `Vertex2D`
struct SEQUOIA_RENDER_API Vertex2DLayout : public VertexLayout {
  using VertexType = Vertex2D;
  virtual void accept(VertexVisitor& visitor) const override;
};

//===------------------------------------------------------------------------------------------===//
//    Vertex3D
//===------------------------------------------------------------------------------------------===//

/// @brief Representation of a 3D vertex
struct SEQUOIA_RENDER_API Vertex3D {
  using PositionType = float;
  using NormalType = float;
  using TexCoordType = float;
  using ColorType = unsigned char;

  PositionType Position[3];
  NormalType Normal[3];
  TexCoordType TexCoord[2];
  ColorType Color[4];

  /// @brief Get the layout of the 3D vertices
  /// @note This function is thread-safe.
  static const VertexLayout* getLayout() noexcept;
  
  /// @brief Convert to string
  static std::string toString(const Vertex3D& vertex);
};

/// @brief Layout description of `Vertex3D`
/// @ingroup render
struct SEQUOIA_RENDER_API Vertex3DLayout : public VertexLayout {
  using VertexType = Vertex3D;
  virtual void accept(VertexVisitor& visitor) const override;
};

/// @typedef VertexTypeList
/// @brief List of all vertex types
using VertexTypeList = std::tuple<Vertex2D, Vertex3D>;

/// @}

} // namespace render

} // namespace sequoia

#endif
