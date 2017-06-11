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

#ifndef SEQUOIA_RENDER_VERTEXVISITOR_H
#define SEQUOIA_RENDER_VERTEXVISITOR_H

#include "sequoia/Core/Assert.h"
#include "sequoia/Render/Export.h"
#include "sequoia/Render/Vertex.h"
#include <cstddef>
#include <functional>
#include <string>

namespace sequoia {

namespace render {

/// @brief Visitor for vertices
///
/// This visitor allows to recover the type information of the vertex data, which is usually given
/// as a raw pointer to memory (`verticesPtr`), the number of vertices (`numVertices`) and the
/// VertexLayout of the vertex data (`layout`). Note that this implies that `data` points to
/// `numVertices * layout->SizeOf` bytes.
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
///   VertexData::modify
///
/// @ingroup render
class SEQUOIA_RENDER_API VertexVisitor {
  void* verticesPtr_ = nullptr; ///< Pointer to the vertex data
  std::size_t numVertices_ = 0; ///< Number of vertices

public:
  /// @brief Get the vertex data corresponding to the given layout
  template <class VertexLayoutType, class VertexType = typename VertexLayoutType::VertexType>
  VertexType* getVerticesPtr(const VertexLayoutType*) {
    SEQUOIA_ASSERT_MSG(verticesPtr_, "vertices pointer not set");
    return reinterpret_cast<VertexType*>(verticesPtr_);
  }

  /// @brief Set the vertex data
  void setVerticesPtr(void* verticesPtr) { verticesPtr_ = verticesPtr; }

  /// @brief Get the number of vertices
  std::size_t getNumVertices() const { return numVertices_; }

  /// @brief Set the number of vertices
  void setNumVertices(std::size_t numVertices) { numVertices_ = numVertices; }

  /// @brief Visit Vertex3D
  virtual void visit(const Vertex3DLayout* layout) = 0;

  /// @brief Visit Vertex2D
  virtual void visit(const Vertex2DLayout* layout) = 0;
};

/// @brief Convert vertex-data to string
/// @ingroup render
class SEQUOIA_RENDER_API VertexVisitorStringifier : public VertexVisitor {
  std::string string_;

public:
  virtual void visit(const Vertex3DLayout* layout) override;
  virtual void visit(const Vertex2DLayout* layout) override;

  /// @brief Get the string
  const std::string& toString() const { return string_; }
};

namespace internal {

template <bool RunFunctor>
struct RunFunctorImpl {
  template <class Functor, class VertexType>
  static void run(Functor&& functor, VertexType* vertex) {
    functor(vertex);
  }
};

template <>
struct RunFunctorImpl<false> {
  template <class Functor, class VertexType>
  static void run(Functor&&, VertexType*) {}
};

} // namespace internal

/// @brief Run the given `functor` on the vertex data
/// @ingroup render
template <class VertexType>
class VertexVisitorRunFunctor : public VertexVisitor {
  const std::function<void(VertexType*)>& functor_;

public:
  VertexVisitorRunFunctor(const std::function<void(VertexType*)>& functor) : functor_(functor) {}

  template <class LayoutType>
  void runImpl(const LayoutType* layout) {
    using VertexT = std::remove_pointer_t<decltype(getVerticesPtr(layout))>;
    internal::RunFunctorImpl<std::is_same<VertexT, VertexType>::value>::run(functor_,
                                                                            getVerticesPtr(layout));
  }

  virtual void visit(const Vertex3DLayout* layout) override { runImpl(layout); }
  virtual void visit(const Vertex2DLayout* layout) override { runImpl(layout); }
};

} // namespace render

} // namespace sequoia

#endif
