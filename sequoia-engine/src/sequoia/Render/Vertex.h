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
/// @ingroup
struct VertexLayout {
  std::size_t SizeOf;

  std::size_t PositionSize;
  std::size_t PositionLength;
  std::size_t PositionOffset;

  std::size_t NormalSize;
  std::size_t NormalLength;
  std::size_t NormalOffset;
  
  std::size_t TextureSize;
  std::size_t TextureLength;
  std::size_t TextureOffset;
  
  std::size_t ColorSize;
  std::size_t ColorLength;
  std::size_t ColorOffset;
};

/// @brief Representation of a 3D vertex
/// @ingroup render
struct Vertex3D {
  float Position[3];
  float Normal[3];
  float Texture[2];
  unsigned char Color[4];
  
  static VertexLayout getLayout() noexcept;
};

/// @brief Representation of a 2D vertex
/// @ingroup render
struct Vertex2D {
  float Position[2];
  float Texture[2];
  unsigned char Color[4];
  
  static VertexLayout getLayout() noexcept;
};

} // namespace render

} // namespace sequoia

#endif
