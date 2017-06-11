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

#include "sequoia/Core/Format.h"
#include "sequoia/Core/StringUtil.h"
#include "sequoia/Core/Unreachable.h"
#include "sequoia/Math/Math.h"
#include "sequoia/Render/Vertex.h"
#include "sequoia/Render/VertexVisitor.h"
#include <mutex>
#include <sstream>
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

static const char* typeToString(VertexLayout::Type type) {
  switch(type) {
  case VertexLayout::Invalid:
    return "Invalid";
  case VertexLayout::UnsignedByte:
    return "UnsignedByte";
  case VertexLayout::Float:
    return "Float";
  default:
    sequoia_unreachable("invalid type");
  }
}

} // anonymous namespace

#define SEQUOIA_PRINT_ATTRIBUTE(Name)                                                              \
  if(has##Name()) {                                                                                \
    ss << "  " #Name "Type = " << typeToString(Name##Type)                                         \
       << ",\n  " #Name "Offset = " << Name##Offset                                                \
       << ",\n  " #Name "NumElement = " << Name##NumElement                                        \
       << ",\n  " #Name "Normalized = " << (Name##Normalized ? "true" : "false") << ",\n";         \
  }

std::string VertexLayout::toString() const {
  std::stringstream ss;
  ss << "VertexLayout[\n";

  SEQUOIA_PRINT_ATTRIBUTE(Position);
  SEQUOIA_PRINT_ATTRIBUTE(Normal);
  SEQUOIA_PRINT_ATTRIBUTE(TexCoord);
  SEQUOIA_PRINT_ATTRIBUTE(Color);

  ss << "]";
  return ss.str();
}

#undef SEQUOIA_PRINT_ATTRIBUTE

#define SEQUOIA_SET_LAYOUT(Layout, VertexType, Name, Normalize)                                    \
  using Name##Type_t = decltype(VertexType::Name);                                                 \
  Layout->Name##NumElement = sizeof(Name##Type_t) / sizeof(GetPrimitveType<Name##Type_t>::type);   \
  Layout->Name##Offset = offsetof(VertexType, Name);                                               \
  Layout->Name##Type = getType<Name##Type_t>();                                                    \
  Layout->Name##Normalized = Normalize;

const VertexLayout* Vertex2D::getLayout() noexcept {
  std::call_once(vertex2DInitFlag, [] {
    vertex2DLayout->SizeOf = sizeof(Vertex2D);
    SEQUOIA_SET_LAYOUT(vertex2DLayout, Vertex2D, Position, false);
    SEQUOIA_SET_LAYOUT(vertex2DLayout, Vertex2D, TexCoord, false);
    SEQUOIA_SET_LAYOUT(vertex2DLayout, Vertex2D, Color, true);
  });
  return vertex2DLayout;
}

const VertexLayout* Vertex3D::getLayout() noexcept {
  std::call_once(vertex3DInitFlag, [] {
    vertex3DLayout->SizeOf = sizeof(Vertex3D);
    SEQUOIA_SET_LAYOUT(vertex3DLayout, Vertex3D, Position, false);
    SEQUOIA_SET_LAYOUT(vertex3DLayout, Vertex3D, Normal, false);
    SEQUOIA_SET_LAYOUT(vertex3DLayout, Vertex3D, TexCoord, false);
    SEQUOIA_SET_LAYOUT(vertex3DLayout, Vertex3D, Color, true);
  });
  return vertex3DLayout;
}
#undef SEQUOIA_SET_LAYOUT

std::string Vertex2D::toString(const Vertex2D& vertex) {
  return core::format(
      "Vertex2D[\n"
      "  %-10s = %s,\n"
      "  %-10s = %s,\n"
      "  %-10s = %s\n"
      "]",
      "Position", math::make_vec2(vertex.Position), "TexCoord", math::make_vec2(vertex.TexCoord),
      "Color", math::ivec4(vertex.Color[0], vertex.Color[1], vertex.Color[2], vertex.Color[3]));
}

std::string Vertex3D::toString(const Vertex3D& vertex) {
  return core::format(
      "Vertex3D[\n"
      "  %-10s = %s,\n"
      "  %-10s = %s,\n"
      "  %-10s = %s,\n"
      "  %-10s = %s\n"
      "]",
      "Position", math::make_vec3(vertex.Position), "Normal", math::make_vec3(vertex.Normal),
      "TexCoord", math::make_vec2(vertex.TexCoord), "Color",
      math::ivec4(vertex.Color[0], vertex.Color[1], vertex.Color[2], vertex.Color[3]));
}

void Vertex2DLayout::accept(VertexVisitor& visitor) const { visitor.visit(this); }

void Vertex3DLayout::accept(VertexVisitor& visitor) const { visitor.visit(this); }

} // namespace render

} // namespace sequoia
