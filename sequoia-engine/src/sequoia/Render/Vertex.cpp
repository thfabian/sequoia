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

#include "sequoia/Render/Vertex.h"
#include <mutex>
#include <type_traits>
#include <utility>

namespace sequoia {

namespace render {

namespace {

template <class T>
struct GetPrimitveType {
  using type = typename std::remove_pointer_t<typename std::decay_t<T>>;
};

template <class T>
struct GetTypeImpl {};

template <>
struct GetTypeImpl<float> {
  static constexpr VertexLayout::Type value = VertexLayout::Float;
};

template <>
struct GetTypeImpl<unsigned char> {
  static constexpr VertexLayout::Type value = VertexLayout::UnsignedByte;
};

template <class T>
constexpr VertexLayout::Type getType() {
  return GetTypeImpl<typename GetPrimitveType<T>::type>::value;
}

static VertexLayout* Vertex2DLayout = new VertexLayout;
static std::once_flag Vertex2DInitFlag;

static VertexLayout* Vertex3DLayout = new VertexLayout;
static std::once_flag Vertex3DInitFlag;

} // anonymous namespace

#define SEQUOIA_SET_LAYOUT(VertexType, Name)                                                       \
  using Name##Type_t = decltype(VertexType::Name);                                                 \
  VertexType##Layout->Name##NumElement =                                                           \
      sizeof(Name##Type_t) / sizeof(GetPrimitveType<Name##Type_t>::type);                          \
  VertexType##Layout->Name##Offset = offsetof(VertexType, Name);                                   \
  VertexType##Layout->Name##Type = getType<Name##Type_t>();

const VertexLayout* Vertex2D::getLayout() noexcept {
  std::call_once(Vertex2DInitFlag, [] {
    Vertex2DLayout->SizeOf = sizeof(Vertex2D);
    SEQUOIA_SET_LAYOUT(Vertex2D, Position);
    SEQUOIA_SET_LAYOUT(Vertex2D, TexCoord);
    SEQUOIA_SET_LAYOUT(Vertex2D, Color);
  });
  return Vertex2DLayout;
}

const VertexLayout* Vertex3D::getLayout() noexcept {
  std::call_once(Vertex3DInitFlag, [] {
    Vertex3DLayout->SizeOf = sizeof(Vertex3D);
    SEQUOIA_SET_LAYOUT(Vertex3D, Position);
    SEQUOIA_SET_LAYOUT(Vertex3D, Normal);
    SEQUOIA_SET_LAYOUT(Vertex3D, TexCoord);
    SEQUOIA_SET_LAYOUT(Vertex3D, Color);
  });
  return Vertex3DLayout;
}

#undef SEQUOIA_SET_LAYOUT

} // namespace render

} // namespace sequoia
