
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
#include "sequoia-engine/Core/Color.h"
#include "sequoia-engine/Core/Unreachable.h"
#include "sequoia-engine/Math/Math.h"
#include "sequoia-engine/Render/Vertex.h"
#include <algorithm>
#include <array>
#include <cstring>
#include <iosfwd>

namespace sequoia {

namespace render {

namespace internal {

#define SEQUOIA_PP_VA_MAX_SIZEOF(r, Data, Vertex)                                                  \
  maxSize = Vertex::getLayout().SizeOf > maxSize ? Vertex::getLayout().SizeOf : maxSize;

/// @brief Compute the maximum size (in bytes) of any Vertex
inline constexpr std::size_t computeMaxVertexSize() {
  std::size_t maxSize = 0;
  BOOST_PP_SEQ_FOR_EACH(SEQUOIA_PP_VA_MAX_SIZEOF, Data, SEQUOIA_VERTICES);
  return maxSize;
}

} // namespace internal

/// @brief Allow unified access to all vertices
///
/// This data structure is fairly efficient and only operates on the stack.
///
/// @ingroup render
class SEQUOIA_API VertexAdapter {
public:
  /// @brief Initialize the adapter by copying the data of `vertex`
  template <class T, class = std::enable_if_t<!std::is_same<T, VertexLayout>::type>>
  VertexAdapter(const T& vertex) : layout_(vertex.getLayout()) {
    copyFrom(&vertex);
  }

  /// @brief Allocate memory for a vertex with the given `layout`
  ///
  /// To fill the adapter with an existing vertex pointer, use `VertexAdapter::copyFrom()`.
  VertexAdapter(VertexLayout layout) : layout_(std::move(layout)) { clear(); }

  /// @brief Set `data` as the new attribute
  ///
  /// If the vertex does not contain the attribute, this function does nothing.
  ///
  /// @param data           Pointer to `size` elements.
  /// @param numElements    Number of elements to set. If the vertex attribute contains less than
  ///                       `numElements` elements, the remaining elements of `data` will be
  ///                       ignored. In case the vertex attribute contains more than `numElements`
  ///                       elements, the ramining elements will be set to 0.
  /// @{
  inline void setPosition(const float* data, int numElements) noexcept {
    setAttribute(layout_.Position, data, numElements);
  }
  inline void setPosition(const math::vec2& data) noexcept {
    setPosition(math::value_ptr(data), data.length());
  }
  inline void setPosition(const math::vec3& data) noexcept {
    setPosition(math::value_ptr(data), data.length());
  }
  inline void setPosition(const math::vec4& data) noexcept {
    setPosition(math::value_ptr(data), data.length());
  }

  inline void setNormal(const float* data, int numElements) noexcept {
    setAttribute(layout_.Normal, data, numElements);
  }
  inline void setNormal(const math::vec2& data) noexcept {
    setNormal(math::value_ptr(data), data.length());
  }
  inline void setNormal(const math::vec3& data) noexcept {
    setNormal(math::value_ptr(data), data.length());
  }
  inline void setNormal(const math::vec4& data) noexcept {
    setNormal(math::value_ptr(data), data.length());
  }

  inline void setTexCoord(const float* data, int numElements) noexcept {
    setAttribute(layout_.TexCoord, data, numElements);
  }
  inline void setTexCoord(const math::vec2& data) noexcept {
    setTexCoord(math::value_ptr(data), data.length());
  }
  inline void setTexCoord(const math::vec3& data) noexcept {
    setTexCoord(math::value_ptr(data), data.length());
  }
  inline void setTexCoord(const math::vec4& data) noexcept {
    setTexCoord(math::value_ptr(data), data.length());
  }

  inline void setColor(const Byte* data, int numElements) noexcept {
    setAttribute(layout_.Color, data, numElements);
  }
  inline void setColor(const Color& color) noexcept { setColor(color.data, Color::NumChannels); }
  /// @}

  /// @brief Get a copy of the vertex attribute
  ///
  /// Note that for the Position, Normal and TexCoord a 4 element float vector is returned in any
  /// case - if the attribute contains less than 4 elements, the remaining elements will be filled
  /// with 0. The Color will be default constructed and filled with the color vertex attribute if
  /// possible.
  /// @{
  inline math::vec4 getPosition() const noexcept {
    math::vec4 data(0);
    getAttribute(layout_.Position, math::value_ptr(data), data.length());
    return data;
  }
  inline math::vec4 getNormal() const noexcept {
    math::vec4 data(0);
    getAttribute(layout_.Normal, math::value_ptr(data), data.length());
    return data;
  }
  inline math::vec4 getTexCoord() const noexcept {
    math::vec4 data(0);
    getAttribute(layout_.TexCoord, math::value_ptr(data), data.length());
    return data;
  }
  inline Color getColor() const noexcept {
    Color data;
    getAttribute(layout_.Color, data.data, Color::NumChannels);
    return data;
  }
  /// @}

  /// @brief Copy the vertex data to `dest`
  ///
  /// Note that sufficient memory (i.e `[dest, dest + getLayout().SizeOf)` needs to be allocated
  /// starting at `dest`.
  void copyTo(void* dest) const noexcept { std::memcpy(dest, vertexData_.data(), layout_.SizeOf); }

  /// @brief Copy the vertex data from `src`
  void copyFrom(const void* src) noexcept { std::memcpy(vertexData_.data(), src, layout_.SizeOf); }

  /// @brief Clear the internal buffer
  void clear() noexcept { vertexData_.fill(0); }

  /// @brief Get the vertex data layout
  const VertexLayout& getLayout() const noexcept { return layout_; }

  /// @brief Check if `Position` attribute is available
  bool hasPosition() const { return layout_.hasPosition(); }

  /// @brief Check if `Normal` attribute is available
  bool hasNormal() const { return layout_.hasNormal(); }

  /// @brief Check if `TexCoord` attribute is available
  bool hasTexCoord() const { return layout_.hasTexCoord(); }

  /// @brief Check if `Color` attribute is available
  bool hasColor() const { return layout_.hasColor(); }

  /// @brief Convert to string
  std::string toString() const;

private:
  /// @brief Set the `attribute` to be equal to the array `data` of size `numElements`
  template <class T>
  inline void setAttribute(const VertexLayout::Attribute& attribute, const T* data,
                           const int numElements) noexcept {
    if(attribute.NumElements == 0)
      return;

    Byte* vertexData = vertexData_.data() + attribute.Offset;
    const int vertexNumElements = attribute.NumElements;

    switch(attribute.Type) {

#define VERTEX_LAYOUT_TYPE(Type, Name)                                                             \
  case VertexLayout::Name:                                                                         \
    setArray<VertexLayout::Name>(vertexData, vertexNumElements, data, numElements);                \
    break;
#include "sequoia-engine/Render/VertexLayout.inc"
#undef VERTEX_LAYOUT_TYPE
    case VertexLayout::Invalid:
    default:
      sequoia_unreachable("invalid type");
    }
  }

  /// @brief Interpret `vertexNumElements` starting at `vertexData` as of type `TypeID` and set it
  /// to be equal to the array `data` of size `numElements`
  template <VertexLayout::TypeID TypeID, class DataType>
  inline void setArray(Byte* vertexData, const int vertexNumElements, const DataType* data,
                       const int numElements) noexcept {
    using VertexDataType = typename internal::TypeIDToType<TypeID>::type;
    const int numElementsToCopy = std::min(vertexNumElements, numElements);

    for(int i = 0; i < numElementsToCopy; ++i, vertexData += sizeof(VertexDataType))
      set<VertexDataType>(vertexData, data[i]);

    for(int i = numElementsToCopy; i < vertexNumElements; ++i, vertexData += sizeof(VertexDataType))
      setToZero<VertexDataType>(vertexData);
  }

  /// @brief Interpret `vertexData` as type `T` and set it to be equal to `value`
  template <class T, class U>
  inline static void set(Byte* vertexData, const U& value) noexcept {
    T& left = *reinterpret_cast<T*>(vertexData);
    left = static_cast<T>(value);
  }

  /// @brief Interpret `vertexData` as type `T` and set it to 0
  template <class T>
  inline static void setToZero(Byte* vertexData) noexcept {
    T& left = *reinterpret_cast<T*>(vertexData);
    left = 0;
  }

  /// @brief Copy the the `attribute` to the array `data` of size `numElements`
  template <class T>
  inline void getAttribute(const VertexLayout::Attribute& attribute, T* data,
                           const int numElements) const noexcept {
    if(attribute.NumElements == 0)
      return;

    const Byte* vertexData = vertexData_.data() + attribute.Offset;
    const int vertexNumElements = attribute.NumElements;

    switch(attribute.Type) {
#define VERTEX_LAYOUT_TYPE(Type, Name)                                                             \
  case VertexLayout::Name:                                                                         \
    getArray<VertexLayout::Name>(vertexData, vertexNumElements, data, numElements);                \
    break;
#include "sequoia-engine/Render/VertexLayout.inc"
#undef VERTEX_LAYOUT_TYPE
    case VertexLayout::Invalid:
    default:
      sequoia_unreachable("invalid type");
    }
  }

  /// @brief Interpret `vertexNumElements` starting at `vertexData` as of type `TypeID` and copy it
  /// to the array `data` of size `numElements`
  template <VertexLayout::TypeID TypeID, class DataType>
  inline static void getArray(const Byte* vertexData, const int vertexNumElements, DataType* data,
                              const int numElements) noexcept {
    using VertexDataType = typename internal::TypeIDToType<TypeID>::type;
    const int numElementsToCopy = std::min(vertexNumElements, numElements);

    for(int i = 0; i < numElementsToCopy; ++i, vertexData += sizeof(VertexDataType))
      get<VertexDataType>(vertexData, data + i);
  }

  /// @brief Interpret `vertexData` as type `T` and assign it to `value`
  template <class T, class U>
  inline static void get(const Byte* vertexData, U* value) noexcept {
    const T& right = *reinterpret_cast<const T*>(vertexData);
    *value = static_cast<U>(right);
  }

private:
  VertexLayout layout_; ///< Layout description of the vertex
  std::array<Byte, internal::computeMaxVertexSize()> vertexData_; ///< Vertex data
};

SEQUOIA_API extern std::ostream& operator<<(std::ostream& os, const VertexAdapter& adapter);

} // namespace render

} // namespace sequoia

#endif
