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

namespace sequoia {

namespace render {

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

#endif
