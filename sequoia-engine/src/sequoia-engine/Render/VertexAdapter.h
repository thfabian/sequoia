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

#ifndef SEQUOIA_ENGINE_RENDER_VERTEXADAPTER_H
#define SEQUOIA_ENGINE_RENDER_VERTEXADAPTER_H

#include "sequoia-engine/Core/Byte.h"
#include "sequoia-engine/Core/Unreachable.h"
#include "sequoia-engine/Math/Math.h"
#include "sequoia-engine/Render/Vertex2.h"
#include <algorithm>
#include <cstring>

namespace sequoia {

namespace render {

namespace internal {

#define SEQUOIA_PP_VA_MAX_SIZEOF(r, Data, Vertex)                                                  \
  maxSize = Vertex::getLayout().SizeOf > maxSize ? Vertex::getLayout().SizeOf : maxSize;

/// @brief Compute the maximum size (in bytes) of any Vertex
static constexpr std::size_t computeMaxVertexSize() {
  std::size_t maxSize = 0;
  BOOST_PP_SEQ_FOR_EACH(SEQUOIA_PP_VA_MAX_SIZEOF, Data, SERUOIA_VERTICES);
  return maxSize;
}

} // namespace internal

/// @brief Allow unified access to all vertices
/// @ingroup render
class SEQUOIA_API VertexAdapter {
public:
  /// @brief Allocate memory for a vertex with the given `layout`
  VertexAdapter(VertexLayout2 layout) : layout_(std::move(layout)), vertexData_{0} {}

  /// @brief Set `data` as the new Position data
  ///
  /// If the vertex does not contain a position attribute, this function does nothing.
  ///
  /// @param data    Pointer to `size` elements.
  /// @param size    Number of elements to set. If the vertex attribute contains less than `size`
  ///                elements, the remaining elements of `data` will be ignored. In case the vertex
  ///                attribute contains more than `size` elements, the ramining elements will be
  ///                set to 0.
  /// @{
  void setPosition(const float* data, int numElements) noexcept {
    setAttribute(layout_.Position, data, numElements);
  }
  void setPosition(const math::vec3& data) noexcept {
    setPosition(math::value_ptr(data), data.length());
  }
  void setPosition(const math::vec2& data) noexcept {
    setPosition(math::value_ptr(data), data.length());
  }
  /// @}

  /// @brief Copy the vertex data to `dest`
  ///
  /// Note that sufficient memory (i.e `[dest, dest + getLayout().SizeOf)` needs to be allocated.
  void copyTo(void* dest) const noexcept { std::memcpy(dest, vertexData_, layout_.SizeOf); }

  /// @brief Get the vertex data layout
  const VertexLayout2& getLayout() const noexcept { return layout_; }

private:
  ///
  template <class T>
  inline void setAttribute(const VertexLayout2::Attribute& attribute, const T* data,
                           const int numElements) noexcept {
    if(attribute.NumElements == 0)
      return;

    Byte* vertexData = vertexData_ + attribute.Offset;
    const int vertexNumElements = attribute.NumElements;

    switch(attribute.Type) {
    case VertexLayout2::UInt8:
      setArray<VertexLayout2::UInt8>(vertexData, vertexNumElements, data, numElements);
      break;
    case VertexLayout2::Float32:
      setArray<VertexLayout2::Float32>(vertexData, vertexNumElements, data, numElements);
      break;
    case VertexLayout2::Invalid:
    default:
      sequoia_unreachable("invalid type");
    }
  }

  ///
  template <VertexLayout2::TypeID TypeID, class DataType>
  inline static void setArray(Byte* vertexData, const int vertexNumElements, const DataType* data,
                              const int numElements) noexcept {
    using VertexDataType = typename internal::TypeIDToType<TypeID>::type;

    const int numElementsToCopy = std::min(vertexNumElements, numElements);

    for(int i = 0; i < numElementsToCopy; ++i, vertexData += sizeof(VertexDataType))
      set<VertexDataType>(vertexData, data[i]);

    for(int i = numElementsToCopy; i < vertexNumElements; ++i, vertexData += sizeof(VertexDataType))
      setToZero<VertexDataType>(vertexData);
  }

  ///
  template <class T, class U>
  inline static void set(Byte* vertexData, const U& right) noexcept {
    T& left = *reinterpret_cast<T*>(vertexData);
    left = static_cast<T>(right);
  }

  ///
  template <class T>
  inline static void setToZero(Byte* vertexData) noexcept {
    T& left = *reinterpret_cast<T*>(vertexData);
    left = 0;
  }

private:
  VertexLayout2 layout_;                              ///< Layout description of the vertex
  Byte vertexData_[internal::computeMaxVertexSize()]; ///< Stack allocated data of the vertex
};

} // namespace render

} // namespace sequoia

#endif
