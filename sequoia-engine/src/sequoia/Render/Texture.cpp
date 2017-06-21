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

#include "sequoia/Core/Format.h"
#include "sequoia/Core/Unreachable.h"
#include "sequoia/Render/Texture.h"
#include <boost/functional/hash.hpp>

namespace sequoia {

namespace render {

namespace {

const char* FilterKindToString(TextureParameter::FilterKind kind) noexcept {
  switch(kind) {
  case TextureParameter::FK_Linear:
    return "Linear";
  case TextureParameter::FK_Nearest:
    return "Nearest";
  default:
    sequoia_unreachable("invalid FilterKind");
  }
}

const char* EdgeSamplingKindToString(TextureParameter::EdgeSamplingKind kind) noexcept {
  switch(kind) {
  case TextureParameter::EK_Repeat:
    return "Linear";
  case TextureParameter::EK_MirroredRepeat:
    return "Nearest";
  case TextureParameter::EK_ClampToBorder:
    return "ClampToBorder";
  case TextureParameter::EK_ClampToEdge:
    return "ClampToEdge";
  default:
    sequoia_unreachable("invalid EdgeSamplingKind");
  }
}

} // anonymous namespace

Texture::Texture(RenderSystemKind kind) : RenderSystemObject(kind) {}

Texture::~Texture() {}

bool TextureParameter::operator==(const TextureParameter& other) const noexcept {
  return Dim == other.Dim &&                                             //
         MinFilter == other.MinFilter &&                                 //
         MaxFilter == other.MaxFilter &&                                 //
         UseMipmap == other.UseMipmap &&                                 //
         InterpolateBetweenMipmaps == other.InterpolateBetweenMipmaps && //
         Dim1EdgeSampling == other.Dim1EdgeSampling &&                   //
         Dim2EdgeSampling == other.Dim2EdgeSampling &&                   //
         Dim3EdgeSampling == other.Dim3EdgeSampling;
}

std::string TextureParameter::toString() const {
  return core::format("TextureParameter[\n"
                      "  Dim = %s,\n"
                      "  MinFilter = %s,\n"
                      "  MaxFilter = %s,\n"
                      "  UseMipmap = %s,\n"
                      "  InterpolateBetweenMipmaps = %s,\n"
                      "  Dim1EdgeSampling = %s,\n"
                      "  Dim2EdgeSampling = %s,\n"
                      "  Dim3EdgeSampling = %s\n"
                      "]",
                      Dim, FilterKindToString(MinFilter), FilterKindToString(MaxFilter),
                      UseMipmap ? "true" : "false", InterpolateBetweenMipmaps ? "true" : "false",
                      EdgeSamplingKindToString(Dim1EdgeSampling),
                      EdgeSamplingKindToString(Dim2EdgeSampling),
                      EdgeSamplingKindToString(Dim3EdgeSampling));
}

} // namespace render

} // namespace sequoia

namespace std {

template <class T, class... Args>
void hash_combine(std::size_t& seed, T&& arg) {
  boost::hash_combine(seed, arg);
}

template <class T, class... Args>
void hash_combine(std::size_t& seed, T&& arg, Args&&... args) {
  boost::hash_combine(seed, arg);
  hash_combine(seed, args...);
}

std::size_t hash<sequoia::render::TextureParameter>::
operator()(const sequoia::render::TextureParameter& param) const {
  std::size_t seed = 0;
  hash_combine(seed, param.Dim, param.MinFilter, param.MaxFilter, param.UseMipmap,
               param.InterpolateBetweenMipmaps, param.Dim1EdgeSampling, param.Dim2EdgeSampling,
               param.Dim3EdgeSampling);
  return seed;
}

} // namespace std
