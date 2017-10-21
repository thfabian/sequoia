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

#include "sequoia/Render/Texture.h"
#include "sequoia/Core/Format.h"
#include "sequoia/Core/StringUtil.h"
#include "sequoia/Core/Unreachable.h"
#include <vector>

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

const char* TextureKindToString(TextureParameter::TextureKind kind) noexcept {
  switch(kind) {
  case TextureParameter::TK_1D:
    return "1D";
  case TextureParameter::TK_2D:
    return "2D";
  case TextureParameter::TK_3D:
    return "3D";
  case TextureParameter::TK_CubeMap:
    return "CubeMap";
  default:
    sequoia_unreachable("invalid EdgeSamplingKind");
  }
}

std::string UsageKindToString(TextureParameter::UsageKind kind) noexcept {
  std::vector<const char*> usages;

  if(kind & TextureParameter::UK_Static)
    usages.push_back("Static");
  if(kind & TextureParameter::UK_Dynamic)
    usages.push_back("Dynamic");
  if(kind & TextureParameter::UK_WriteOnly)
    usages.push_back("WriteOnly");
  if(kind & TextureParameter::UK_Discardable)
    usages.push_back("Discardable");
  if(kind & TextureParameter::UK_Rendertarget)
    usages.push_back("Rendertarget");

  return core::RangeToString(", ", "{", "}")(usages);
}

} // anonymous namespace

Texture::Texture(RenderSystemKind kind) : RenderRessource(kind) {}

Texture::~Texture() {}

bool TextureParameter::operator==(const TextureParameter& other) const noexcept {
  return Kind == other.Kind &&                                           //
         Usage == other.Usage &&                                         //
         MinFilter == other.MinFilter &&                                 //
         MagFilter == other.MagFilter &&                                 //
         UseMipmap == other.UseMipmap &&                                 //
         InterpolateBetweenMipmaps == other.InterpolateBetweenMipmaps && //
         Dim1EdgeSampling == other.Dim1EdgeSampling &&                   //
         Dim2EdgeSampling == other.Dim2EdgeSampling &&                   //
         Dim3EdgeSampling == other.Dim3EdgeSampling;
}

std::string TextureParameter::toString() const {
  return core::format(
      "TextureParameter[\n"
      "  Kind = {},\n"
      "  Usage = {},\n"
      "  MinFilter = {},\n"
      "  MagFilter = {},\n"
      "  UseMipmap = {},\n"
      "  InterpolateBetweenMipmaps = {},\n"
      "  Dim1EdgeSampling = {},\n"
      "  Dim2EdgeSampling = {},\n"
      "  Dim3EdgeSampling = {}\n"
      "]",
      TextureKindToString(Kind), UsageKindToString(Usage), FilterKindToString(MinFilter),
      FilterKindToString(MagFilter), UseMipmap ? "true" : "false",
      InterpolateBetweenMipmaps ? "true" : "false", EdgeSamplingKindToString(Dim1EdgeSampling),
      EdgeSamplingKindToString(Dim2EdgeSampling), EdgeSamplingKindToString(Dim3EdgeSampling));
}

} // namespace render

} // namespace sequoia
