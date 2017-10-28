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

#ifndef SEQUOIA_ENGINE_RENDER_VERTEXVISITOR_H
#define SEQUOIA_ENGINE_RENDER_VERTEXVISITOR_H

#include "sequoia-engine/Core/Assert.h"
#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Render/Vertex.h"
#include <boost/preprocessor/seq/for_each.hpp>
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
/// Consider the following example which sets the first component of the position of all the
/// vertices in the vertex data to `0`.
/// @code{.cpp}
///   class VertexSetter : public VertexVisitor {
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
class SEQUOIA_API VertexVisitor {
  void* verticesPtr_ = nullptr; ///< Pointer to the vertex data
  std::size_t numVertices_ = 0; ///< Number of vertices

public:
  virtual ~VertexVisitor() {}

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

#define SEQUOIA_VERTEXVISITOR_IMPL(r, data, elem) virtual void visit(const elem* layout) = 0;

  BOOST_PP_SEQ_FOR_EACH(SEQUOIA_VERTEXVISITOR_IMPL, _, SEQUOIA_VERTEX_LAYOUT_SEQ)

#undef SEQUOIA_VERTEXVISITOR_IMPL
};

/// @brief VertexVisitor which passes through by default
/// @ingroup render
class SEQUOIA_API VertexVisitorPassthrough : public VertexVisitor {
public:
#define SEQUOIA_VERTEXVISITOR_IMPL(r, data, elem) virtual void visit(const elem* layout) override{};

  BOOST_PP_SEQ_FOR_EACH(SEQUOIA_VERTEXVISITOR_IMPL, _, SEQUOIA_VERTEX_LAYOUT_SEQ)

#undef SEQUOIA_VERTEXVISITOR_IMPL
};

/// @brief Convert vertex-data to string
/// @ingroup render
class SEQUOIA_API VertexVisitorStringifier final : public VertexVisitor {
  std::string string_;

public:
  virtual ~VertexVisitorStringifier() {}

#define SEQUOIA_VERTEXVISITOR_IMPL(r, data, elem) virtual void visit(const elem* layout) override;

  BOOST_PP_SEQ_FOR_EACH(SEQUOIA_VERTEXVISITOR_IMPL, _, SEQUOIA_VERTEX_LAYOUT_SEQ)

#undef SEQUOIA_VERTEXVISITOR_IMPL

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
class VertexVisitorRunFunctor final : public VertexVisitor {
  const std::function<void(VertexType*)>& functor_;

public:
  VertexVisitorRunFunctor(const std::function<void(VertexType*)>& functor) : functor_(functor) {}

  template <class LayoutType>
  void runImpl(const LayoutType* layout) {
    using VertexT = std::remove_pointer_t<decltype(getVerticesPtr(layout))>;
    internal::RunFunctorImpl<std::is_same<VertexT, VertexType>::value>::run(functor_,
                                                                            getVerticesPtr(layout));
  }

#define SEQUOIA_VERTEXVISITOR_IMPL(r, data, elem)                                                  \
  virtual void visit(const elem* layout) override { runImpl(layout); }

  BOOST_PP_SEQ_FOR_EACH(SEQUOIA_VERTEXVISITOR_IMPL, _, SEQUOIA_VERTEX_LAYOUT_SEQ)

#undef SEQUOIA_VERTEXVISITOR_IMPL
};

} // namespace render

} // namespace sequoia

#endif
