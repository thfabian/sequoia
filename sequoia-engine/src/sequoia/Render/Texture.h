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

#ifndef SEQUOIA_RENDER_TEXTURE_H
#define SEQUOIA_RENDER_TEXTURE_H

#include "sequoia/Core/Export.h"
#include "sequoia/Core/Image.h"
#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Render/RenderSystemObject.h"
#include <functional>

namespace sequoia {

namespace render {

/// @brief Parameters used to construct the Texture
/// @ingroup render
struct SEQUOIA_API TextureParameter {

  /// @brief Filter used when minifying
  enum FilterKind : std::int8_t {
    FK_Nearest = 0, ///< Select the texel nearest to the texture coordinate
    FK_Linear       ///< Perform a weighted linear blend between the nearest adjacent samples
  };

  /// @brief Sampling used for edges
  ///
  /// Normalized texture coordinates are not limited to values between 0.0 and 1.0. They can be any
  /// floating-point number. When a texture coordinate is not within the [0, 1] range, a heuristic
  /// must be employed to decide what the color value will be.
  enum EdgeSamplingKind : std::int8_t {
    EK_Repeat = 0, ///< The texture coordinate wraps around the texture. So a texture coordinate of
                   ///-0.2 becomes the equivalent of 0.8.
    EK_MirroredRepeat, ///< The texture coordinate wraps around like a mirror. -0.2 becomes 0.2,
                       ///-1.2 becomes 0.8, etc.
    EK_ClampToEdge,    ///< The texture coordinate is clamped to the [0, 1] range.
    EK_ClampToBorder,  ///< The texture coordinate is clamped to the [0, 1] range, but the edge
                       ///  texels are blended with a constant border color.
  };

  /// Is this a compressed texture (.dds)
  bool IsCompressed = false;

  /// Dimensionality of the texture
  int Dim = 2;

  /// Filter used when minifying the texture
  ///
  /// Minification means that the area of the fragment in texture space is larger than a texel.
  FilterKind MinFilter = FK_Linear;

  /// Filter used when magnifying the texture
  ///
  /// Magnification means that the area of the fragment in texture space is smaller than a texel.
  FilterKind MaxFilter = FK_Linear;

  /// Use mipmap for minification?
  bool UseMipmap = true;

  /// Interpolate between two mipmap levels (requires `UseMipmap == true`)
  bool InterpolateBetweenMipmaps = true;

  /// Edge sampling used for the first dimension
  EdgeSamplingKind Dim1EdgeSampling = EK_Repeat;

  /// Edge sampling used for the first dimension
  EdgeSamplingKind Dim2EdgeSampling = EK_Repeat;

  /// Edge sampling used for the first dimension
  EdgeSamplingKind Dim3EdgeSampling = EK_Repeat;

  /// @name Construts
  /// @{
  TextureParameter() = default;
  TextureParameter(const TextureParameter&) = default;
  TextureParameter(TextureParameter&&) = default;

  TextureParameter& operator=(const TextureParameter&) = default;
  TextureParameter& operator=(TextureParameter&&) = default;
  /// @}

  /// @name Comparison
  /// @{
  bool operator==(const TextureParameter& other) const noexcept;
  bool operator!=(const TextureParameter& other) const noexcept { return !(other == *this); }
  /// @}

  /// @brief Convert to string
  std::string toString() const;
};

/// @brief Texture object
///
/// @see RenderSystem::loadTexture
/// @ingroup render
class SEQUOIA_API Texture : public RenderSystemObject,
                            public NonCopyable,
                            public std::enable_shared_from_this<Texture> {
public:
  Texture(RenderSystemKind kind);
  virtual ~Texture();

  /// @brief Get the underlying file of the Texture
  virtual const std::shared_ptr<File>& getFile() const = 0;

  /// @brief Get a log of the shader
  virtual std::string getLog() const = 0;

  /// @brief Convert to string
  virtual std::string toString() const = 0;
};

} // render

} // namespace sequoia

namespace std {

template <>
struct hash<sequoia::render::TextureParameter> {
  std::size_t operator()(const sequoia::render::TextureParameter& param) const;
};

} // namespace std

#endif
