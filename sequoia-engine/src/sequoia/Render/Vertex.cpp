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

static Vertex2DLayout* vertex2DLayout = new Vertex2DLayout;
static std::once_flag vertex2DInitFlag;

static Vertex3DLayout* vertex3DLayout = new Vertex3DLayout;
static std::once_flag vertex3DInitFlag;

} // anonymous namespace

#define SEQUOIA_SET_LAYOUT(Layout, VertexType, Name)                                               \
  using Name##Type_t = decltype(VertexType::Name);                                                 \
  Layout->Name##NumElement = sizeof(Name##Type_t) / sizeof(GetPrimitveType<Name##Type_t>::type);   \
  Layout->Name##Offset = offsetof(VertexType, Name);                                               \
  Layout->Name##Type = getType<Name##Type_t>();

const VertexLayout* Vertex2D::getLayout() noexcept {
  std::call_once(vertex2DInitFlag, [] {
    vertex2DLayout->SizeOf = sizeof(Vertex2D);
    SEQUOIA_SET_LAYOUT(vertex2DLayout, Vertex2D, Position);
    SEQUOIA_SET_LAYOUT(vertex2DLayout, Vertex2D, TexCoord);
    SEQUOIA_SET_LAYOUT(vertex2DLayout, Vertex2D, Color);
  });
  return vertex2DLayout;
}

const VertexLayout* Vertex3D::getLayout() noexcept {
  std::call_once(vertex3DInitFlag, [] {
    vertex3DLayout->SizeOf = sizeof(Vertex3D);
    SEQUOIA_SET_LAYOUT(vertex3DLayout, Vertex3D, Position);
    SEQUOIA_SET_LAYOUT(vertex3DLayout, Vertex3D, Normal);
    SEQUOIA_SET_LAYOUT(vertex3DLayout, Vertex3D, TexCoord);
    SEQUOIA_SET_LAYOUT(vertex3DLayout, Vertex3D, Color);
  });
  return vertex3DLayout;
}

#undef SEQUOIA_SET_LAYOUT

void Vertex3DLayout::accept(VertexVisitor& visitor) const { visitor.visit(this); }

void Vertex2DLayout::accept(VertexVisitor& visitor) const { visitor.visit(this); }

} // namespace render

} // namespace sequoia
