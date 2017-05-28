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

#ifndef SEQUOIA_RENDER_VERTEXDATA_H
#define SEQUOIA_RENDER_VERTEXDATA_H

#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Core/STLExtras.h"
#include "sequoia/Math/AxisAlignedBox.h"
#include "sequoia/Render/Vertex.h"
#include "sequoia/Render/VertexArrayObject.h"
#include "sequoia/Render/VertexVisitor.h"

#include <iostream>

namespace sequoia {

namespace render {

/// @brief Storage of vertex data on the host and device
/// @ingroup render
class SEQUOIA_RENDER_API VertexData : public NonCopyable {
public:
  using IndicesType = unsigned int;

  /// @brief Allocate memory for `numVertices` and `numIndices`
  /// @throw RenderException  Out of memory
  VertexData(const VertexLayout* layout, std::size_t numVertices, std::size_t numIndices = 0);

  /// @brief Deallocate all memory
  ~VertexData();

  /// @brief Get the pointer to the host vertex data
  const void* getVerticesPtr() const { return verticesPtr_; }
  void* getVerticesPtr() { return verticesPtr_; }

  /// @brief Get the number of vertices
  std::size_t getNumVertices() const { return numVertices_; }

  /// @brief Get the layout of the vertex-data
  const VertexLayout* getLayout() const { return layout_; }

  /// @brief Get the number of indices
  std::size_t getNumIndices() const { return numIndices_; }

  /// @brief Do we draw with indices?
  bool hasIndices() const { return (indicesPtr_ != nullptr); }

  /// @brief Set the vertex array object
  ///
  /// This takes ownership of the VAO and allocate a the hardware buffers
  void setVertexArrayObject(std::unique_ptr<VertexArrayObject> vao,
                            VertexArrayObject::BufferUsageKind usage);

  /// @brief Get the vertex array object
  const VertexArrayObject* getVertexArrayObject() const { return vao_.get(); }
  VertexArrayObject* getVertexArrayObject() { return vao_.get(); }

  /// @brief Check if a vertex array object has been attached
  bool hasVertexArrayObject() const { return (vao_ != nullptr); }

  /// @brief Set the local bounding box volume
  void setAxisAlignedBox(const math::AxisAlignedBox& aab) { aab_ = aab; }

  /// @brief Get the local bounding box volume
  const math::AxisAlignedBox& getAxisAlignedBox() const { return aab_; }
  math::AxisAlignedBox& getAxisAlignedBox() { return aab_; }

  /// @brief Get the indices vector
  const IndicesType* getIndicesPtr() const { return indicesPtr_; }
  IndicesType* getIndicesPtr() { return indicesPtr_; }

  /// @brief Accept a VertexVisitor to access/modify the underlying host vertex data
  void accept(VertexVisitor& visitor) const;

  /// @brief Modify the vertex data by running the provided `functor` on it
  ///
  /// @tparam FunctorType   (lambda) function of type `void(VertexType*)` where `VertexType` is one
  ///                       of `{Vertex2D, Vertex3D}`
  /// @param functor        Functor to run on the vertex data
  ///
  /// Use VertexData::accept to provide your own VertexVisitor.
  ///
  /// @b Example
  /// @code{.cpp}
  ///   VertexData data(Vertex3D::getLayout(), 64);
  ///   data.modify([&data](Vertex3D* vertices) {
  ///     for(int i = 0; i < data.getNumVertices(); ++i) {
  ///       Vertex3D vertex = vertices[i];
  ///       // Do something ...
  ///     }
  ///
  ///   });
  /// @endcode
  template <class FunctorType>
  void modify(FunctorType&& functor) const {
    using FirstArgType = core::function_first_argument_t<FunctorType>;

    // First argument has to be VertexType*
    static_assert(std::is_pointer<FirstArgType>::value,
                  "invalid functor: type of first argument has to be a pointer");
    using VertexType = typename std::remove_pointer<FirstArgType>::type;

    // Check if vertex type is supported
    static_assert(core::tuple_has_type<VertexType, VertexTypeList>::value,
                  "invalid functor: invalid 'VertexType' for first argument ");
    std::function<void(VertexType*)> func = functor;

    // Run functor
    VertexVisitorRunFunctor<VertexType> visitor(functor);
    accept(visitor);
  }

  /// @brief Dump the vertex data and indices to `stdout`
  void dump() const;
  
  /// @brief Convert to string
  std::string toString() const;

private:
  /// Host vertex data
  void* verticesPtr_;

  /// Number of vertices
  std::size_t numVertices_;

  /// Device vertex data
  std::unique_ptr<VertexArrayObject> vao_;

  /// Indices
  IndicesType* indicesPtr_;

  /// Number of indices
  std::size_t numIndices_;

  /// Layout of the vertex
  const render::VertexLayout* layout_;

  /// Local bounding box volume
  math::AxisAlignedBox aab_;
};

} // namespace render

} // namespace sequoia

#endif
