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

#include <cstddef>

namespace sequoia {

namespace render {

/// @brief Layout description of vertices
/// @ingroup render
struct VertexLayout {
  enum Type { Invalid, UnsignedByte, Float };

  std::size_t SizeOf = 0;

  Type PositionType = Invalid;
  std::size_t PositionOffset = 0;
  std::size_t PositionNumElement = 0;

  Type NormalType = Invalid;
  std::size_t NormalOffset = 0;
  std::size_t NormalNumElement = 0;

  Type TexCoordType = Invalid;
  std::size_t TexCoordOffset = 0;
  std::size_t TexCoordNumElement = 0;

  Type ColorType = Invalid;
  std::size_t ColorOffset = 0;
  std::size_t ColorNumElement = 0;

  /// @brief Check if `Position` attribute is available
  bool hasPosition() const { return PositionNumElement > 0; }

  /// @brief Check if `Normal` attribute is available
  bool hasNormal() const { return NormalNumElement > 0; }

  /// @brief Check if `TexCoord` attribute is available
  bool hasTexCoord() const { return TexCoordNumElement > 0; }

  /// @brief Check if `Color` attribute is available
  bool hasColor() const { return ColorNumElement > 0; }
};

/// @brief Representation of a 3D vertex
/// @ingroup render
struct Vertex3D {
  float Position[3];
  float Normal[3];
  float TexCoord[2];
  unsigned char Color[4];

  static const VertexLayout* getLayout() noexcept;
};

/// @brief Representation of a 2D vertex
/// @ingroup render
struct Vertex2D {
  float Position[2];
  float TexCoord[2];
  unsigned char Color[4];

  static const VertexLayout* getLayout() noexcept;
};

} // namespace render

} // namespace sequoia

#endif
