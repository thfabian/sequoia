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
#include "sequoia/Core/HashCombine.h"
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

  /// @brief Type of texture
  enum TextureKind : std::int8_t {
    TK_1D = 0,  ///< 1D texture, used in combination with 1D texture coordinates
    TK_2D,      ///< 2D texture, used in combination with 2D texture coordinates (default)
    TK_3D,      ///< 3D volume texture, used in combination with 3D texture coordinates
    TK_CubeMap, ///< Cube map (six two dimensional textures, one for each cube face), used in
                ///  combination with 3D texture coordinates
  };

  /// @brief Usage of the texture (determines allocation strategy)
  enum UsageKind : std::int8_t {
    /// Static texture which the application rarely modifies once created. Modifying the contents
    /// of this texture will involve a performance hit.
    UK_Static = 1,

    /// Indicates the application would like to modify this texture with the CPU fairly often.
    /// This is the least optimal texture setting.
    UK_Dynamic = 2,

    /// Indicates the application will never read the contents of the texture back, it will only
    /// ever write data.
    UK_WriteOnly = 4,

    /// Indicates that the application will be refilling the contents of the texture regularly (not
    /// just updating, but generating the contents from scratch), and therefore does not mind if
    /// the contents of the texture are lost somehow and need to be recreated. This allows and
    /// additional level of optimisation on the texture.
    UK_Discardable = 8,

    /// Combination of `UK_Static` and `UK_WriteOnly`. This is the optimal texture usage setting.
    UK_StaticWriteOnly = 5,

    /// Combination of `UK_Dynamic` and `UK_WriteOnly`.
    UK_DynamicWriteOnly = 6,

    /// Combination of `UK_Dynamic`, `UK_WriteOnly` and `UK_Discardable`. This means that you
    /// expect to replace the entire contents of the texture on an extremely regular basis, most
    /// likely every frame. By selecting this option, you free the system up from having to be
    /// concerned about losing the existing contents of the texture at any time, because if it does
    /// lose them, you will be replacing them next frame anyway.
    UK_DynamicWriteOnlyDiscardable = 14,

    /// This texture will be a render target, i.e. used as a target for render to texture setting
    /// this flag will ignore all other texture usages
    UK_Rendertarget = 16
  };

  /// @brief Type of texture
  TextureKind Kind = TK_2D;

  /// Usage of the texture
  UsageKind Usage = UK_StaticWriteOnly;

  /// @brief Filter used when minifying the texture
  ///
  /// Minification means that the area of the fragment in texture space is larger than a texel.
  FilterKind MinFilter = FK_Linear;

  /// @brief Filter used when magnifying the texture
  ///
  /// Magnification means that the area of the fragment in texture space is smaller than a texel.
  FilterKind MagFilter = FK_Linear;

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

  /// @brief Set the Usage
  inline void setUsage(std::int8_t usageBits) noexcept {
    Usage = static_cast<UsageKind>(usageBits);
  }

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

  /// @brief Get the underlying image of the Texture
  virtual const std::shared_ptr<Image>& getImage() const = 0;

  /// @brief Get the texture parameters
  virtual const std::shared_ptr<TextureParameter>& getParameter() const = 0;

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
  std::size_t operator()(const sequoia::render::TextureParameter& param) const {
    std::size_t seed = 0;
    sequoia::core::hashCombine(seed, param.Kind, param.Usage, param.MinFilter, param.MagFilter,
                               param.UseMipmap, param.InterpolateBetweenMipmaps,
                               param.Dim1EdgeSampling, param.Dim2EdgeSampling,
                               param.Dim3EdgeSampling);
    return seed;
  }
};

template <>
struct hash<std::shared_ptr<sequoia::render::TextureParameter>> {
  std::size_t operator()(const std::shared_ptr<sequoia::render::TextureParameter>& param) const {
    return hash<sequoia::render::TextureParameter>()(*param);
  }
};

} // namespace std

#endif
