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

#ifndef SEQUOIA_RENDER_GLFRAMEBUFFEROBJECT_H
#define SEQUOIA_RENDER_GLFRAMEBUFFEROBJECT_H

#include "sequoia/Render/FrameBufferObject.h"

namespace sequoia {

namespace render {

class GLTexture;

/// @brief OpenGL implemenation of the frame-buffer objects
/// @ingroup gl
class SEQUOIA_API GLFrameBufferObject final : public FrameBufferObject {

  /// OpenGL FBO index
  unsigned int id_;

  /// Texture target
  GLTexture* texture_;

public:
  GLFrameBufferObject();
  virtual ~GLFrameBufferObject();

  /// @brief Check if FBO has been iniailized correctly
  bool isInitialized() const { return id_ != 0; }

  /// @brief Attach a texture
  void attachTexture(GLTexture* texture);

  /// @brief Check if a texture was attached
  bool hasTexture() const { return texture_ != nullptr; }

  /// @brief Get the unique identifer of the program
  ///
  /// Note that IDs might be reused after a program has been destroyed.
  unsigned int getID() const;

  /// @brief Bind the frame buffer object to the current render pipline
  ///
  /// This forces all subsequent render draw calls to render into this FBO.
  void bind();

  /// @brief Unbind the frame buffer object
  void unbind();

  /// @brief Convert to string
  virtual std::string toString() const override;

private:
  /// @brief Initialize the FBO
  void init();
};

} // namespace render

} // namespace sequoia

#endif
