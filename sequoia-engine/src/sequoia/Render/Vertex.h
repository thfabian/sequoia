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

namespace sequoia {

namespace render {

struct VertexLayout;
struct Vertex2DLayout;
struct Vertex3DLayout;
class VertexVisitor;

//===------------------------------------------------------------------------------------------===//
//    VertexLayout
//===------------------------------------------------------------------------------------------===//

/// @brief Layout description of vertices
///
/// Each attribute is described by an array of the form `T Attribute[N]` in the vertex struct with:
///   - @b XType : Type `T` of the attribute (e.g `float`)
///   - @b XOffset : Offset in the vertex struct (in bytes)
///   - @b XNumElement : Number of elements `N` of the attribute
///
/// where @b X is one of `{ Position, Normal, TexCoord, Color}`.
///
/// @ingroup render
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

  /// @brief Accept a VertexVisitor
  virtual void accept(VertexVisitor& visitor) const = 0;
};

//===------------------------------------------------------------------------------------------===//
//    VertexVisitor
//===------------------------------------------------------------------------------------------===//

/// @brief Visitor for vertices
///
/// This visitor allows to recover the type information of the vertex data, which is usually given
/// as a raw pointer to memory (`data`), the number of vertices (`numVertices`) and the VertexLayout
/// of the vertex data (`layout`). Note that this implies that `data` points to `numVertices *
/// layout->SizeOf` bytes.
///
/// The `data` and `numVertices` need to be set before usage. This is done automatically when
/// passing the visitor to a `Mesh`.
///
/// @b Example
/// Consider the following example which set the first component of the position of all the vertices
/// in the vertex data to `0`.
/// @code{.cpp}
///   class VertexPrinter : public VertexVisitor {
///     virtual void visit(Vertex3DLayout* layout) override {
///       Vertex3D* vertices = getDataPtr(layout);
///
///       for(std::size_t i = 0; i < getNumVertices(); ++i)
///         vertices[i][0] = 0.0;
///     }
///   };
/// @endcode
///
/// @see
///   game::Mesh::accept
///
/// @ingroup render
class SEQUOIA_RENDER_API VertexVisitor {
  void* dataPtr_ = nullptr;     ///< Pointer to the vertex data
  std::size_t numVertices_ = 0; ///< Number of vertices

public:
  /// @brief Get the vertex data corresponding to the given layout
  template <class VertexLayoutType, class VertexType = typename VertexLayoutType::VertexType>
  VertexType* getDataPtr(const VertexLayoutType*) {
    SEQUOIA_ASSERT_MSG(dataPtr_, "vertex-data not set!");
    return reinterpret_cast<VertexType*>(dataPtr_);
  }

  /// @brief Set the vertex data
  void setDataPtr(void* dataPtr) { dataPtr_ = dataPtr; }

  /// @brief Get the number of vertices
  std::size_t getNumVertices() const { return numVertices_; }

  /// @brief Set the number of vertices
  void setNumVertices(std::size_t numVertices) { numVertices_ = numVertices; }

  /// @brief Visit Vertex3D
  virtual void visit(const Vertex3DLayout* layout) = 0;

  /// @brief Visit Vertex2D
  virtual void visit(const Vertex2DLayout* layout) = 0;
};

//===------------------------------------------------------------------------------------------===//
//    Vertex3D
//===------------------------------------------------------------------------------------------===//

/// @brief Representation of a 3D vertex
/// @ingroup render
struct SEQUOIA_RENDER_API Vertex3D {
  float Position[3];
  float Normal[3];
  float TexCoord[2];
  float Color[3];

  /// @brief Get the layout of the 3D vertices
  /// @note This function is thread-safe.
  static const VertexLayout* getLayout() noexcept;
};

/// @brief Layout description of `Vertex3D`
/// @ingroup render
struct SEQUOIA_RENDER_API Vertex3DLayout : public VertexLayout {
  using VertexType = Vertex3D;
  virtual void accept(VertexVisitor& visitor) const override;
};

//===------------------------------------------------------------------------------------------===//
//    Vertex2D
//===------------------------------------------------------------------------------------------===//

/// @brief Representation of a 2D vertex
/// @ingroup render
struct SEQUOIA_RENDER_API Vertex2D {
  float Position[2];
  float TexCoord[2];
  unsigned char Color[4];

  /// @brief Get the layout of the 2D vertices
  /// @note This function is thread-safe.
  static const VertexLayout* getLayout() noexcept;
};

/// @brief Layout description of `Vertex2D`
/// @ingroup render
struct SEQUOIA_RENDER_API Vertex2DLayout : public VertexLayout {
  using VertexType = Vertex2D;
  virtual void accept(VertexVisitor& visitor) const override;
};

} // namespace render

} // namespace sequoia

#endif
