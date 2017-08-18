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

#ifndef SEQUOIA_RENDER_VERTEXDATA_H
#define SEQUOIA_RENDER_VERTEXDATA_H

#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Core/STLExtras.h"
#include "sequoia/Render/IndexBuffer.h"
#include "sequoia/Render/RenderSystemObject.h"
#include "sequoia/Render/Vertex.h"
#include "sequoia/Render/VertexBuffer.h"
#include "sequoia/Render/VertexVisitor.h"
#include <functional>

namespace sequoia {

namespace render {

/// @brief Parameter used to initialize the VertexData
/// @ingroup render
struct VertexDataParameter {

  /// Layout of the vertices
  const VertexLayout* Layout;

  /// Number of vertices to allocate
  std::size_t NumVertices;

  /// Number of indices to allocate
  std::size_t NumIndices;

  /// Usage hint of the vertex-buffer
  Buffer::UsageHint VertexBufferUsageHint;

  /// Number of redundant vertex-buffers to allocate
  int NumVertexBuffers = 1;

  /// Use a shadow buffer for the vertices?
  bool UseVertexShadowBuffer = true;

  /// Use a shadow buffer for the indices?
  bool UseIndexShadowBuffer = true;

  /// Type of indices
  IndexBuffer::IndexType IndexType = IndexBuffer::IT_UInt32;

  VertexDataParameter(const VertexLayout* layout, std::size_t numVertices, std::size_t numIndices,
                      Buffer::UsageHint vertexBufferUsageHint)
      : Layout(layout), NumVertices(numVertices), NumIndices(numIndices),
        VertexBufferUsageHint(vertexBufferUsageHint) {}

  VertexDataParameter(const VertexDataParameter&) = default;
  VertexDataParameter(VertexDataParameter&&) = default;
};

/// @brief Manage storage of vertices and indices
/// @ingroup render
class SEQUOIA_API VertexData : public RenderSystemObject, public NonCopyable {
public:
  /// @brief Draw mode
  enum DrawModeKind {
    DM_Triangles = 0 ///< Treats each triplet of vertices as an independent triangle
  };
  
  VertexData(RenderSystemKind renderSystemKind, DrawModeKind drawMode);

  /// @brief Deallocate all memory
  virtual ~VertexData();

  /// @brief Get the draw-mode
  DrawModeKind getDrawMode() const noexcept { return drawMode_; }

  /// @brief Get the layout of the vertices
  const VertexLayout* getLayout() const noexcept { return getVertexBuffer()->getLayout(); }

  /// @brief Get number of allocated vertices
  std::size_t getNumVertices() const noexcept { return getVertexBuffer()->getNumVertices(); }

  /// @brief Check if IndexBuffer is allocated
  bool hasIndices() const { return getIndexBuffer() != nullptr; }

  /// @brief Get number of allocated indices
  std::size_t getNumIndices() const noexcept {
    return getVertexBuffer() ? getVertexBuffer()->getNumVertices() : 0;
  }

  /// @brief Accept a VertexVisitor to write vertex data
  void acceptWriteVisitor(VertexVisitor& visitor) const { accept(visitor, true); }

  /// @brief Accept a read-only visitor of the vertex data
  void acceptReadVisitor(VertexVisitor& visitor) const { accept(visitor, false); }

  /// @brief Obtain the vertex data for reading/writing
  ///
  /// @tparam FunctorType   (lambda) function of type `void(VertexType*)` where `VertexType` is one
  ///                       of `{Vertex2D, Vertex3D}`
  /// @param functor        Functor to run on the vertex data
  ///
  /// @b Example
  /// @code{.cpp}
  ///   VertexData data(...);
  ///   data.write([&data](Vertex3D* vertices) {
  ///     for(int i = 0; i < data.getNumVertices(); ++i) {
  ///       Vertex3D vertex = vertices[i];
  ///       // Do something ...
  ///     }
  ///   });
  /// @endcode
  /// @{
  template <class FunctorType>
  void write(FunctorType&& functor) const {
    modifyImpl<true>(std::forward<FunctorType>(functor));
  }
  template <class FunctorType>
  void read(FunctorType&& functor) const {
    modifyImpl<false>(std::forward<FunctorType>(functor));
  }
  /// @}

  /// @brief Get the VertexBuffer
  virtual VertexBuffer* getVertexBuffer() const = 0;

  /// @brief Get the IndexBuffer
  virtual IndexBuffer* getIndexBuffer() const = 0;

  /// @brief Update the buffer to the next timestep
  virtual void nextTimestep() = 0;
  
  /// @brief Dump the vertex data and indices to `stdout`
  void dump() const;

  /// @brief Convert to string
  std::string toString() const;

protected:
  /// @brief Implementation of `toString` returns stringified members and title
  virtual std::pair<std::string, std::string> toStringImpl() const;

private:
  template <bool IsWrite, class FunctorType>
  void modifyImpl(FunctorType&& functor) const {
    using FirstArgType = core::function_first_argument_t<FunctorType>;

    // First argument has to be `Vertex*` or `const Vertex*`
    static_assert(std::is_pointer<FirstArgType>::value,
                  "invalid functor: type of first argument has to be a pointer");
    using VertexType = typename std::remove_pointer<FirstArgType>::type;

    // Check if vertex type is supported
    static_assert(
        core::tuple_has_type<typename std::remove_cv<VertexType>::type, VertexTypeList>::value,
        "invalid functor: invalid 'VertexType' for first argument ");
    std::function<void(VertexType*)> func = functor;

    // If we read, we expect `const VertexType*`
    static_assert(IsWrite ? true : std::is_const<VertexType>::value,
                  "invalid functor: first argument is missing const for pointer type");

    // Run functor
    VertexVisitorRunFunctor<VertexType> visitor(func);

    if(IsWrite)
      acceptWriteVisitor(visitor);
    else
      acceptReadVisitor(visitor);
  }

  void accept(VertexVisitor& visitor, bool isWrite) const;

private:
  /// Mode of drawing the vertices
  DrawModeKind drawMode_;
};

} // namespace render

} // namespace sequoia

#endif
