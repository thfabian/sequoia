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

  /// OpenGL indices
  unsigned int fboID_, rboDepthID_;

  /// Parameter used for initialization
  FrameBufferObjectParameter param_;

  /// Texture used for rendering
  std::shared_ptr<GLTexture> texture_;
  
public:
  GLFrameBufferObject(const FrameBufferObjectParameter& param);
  virtual ~GLFrameBufferObject();

  /// @brief Get the unique identifer of the program
  ///
  /// Note that IDs might be reused after a program has been destroyed.
  unsigned int getID() const;

  /// @brief Get the parameter used for initialization
  const FrameBufferObjectParameter& getParam() const;

  /// @brief Bind the frame buffer object to the current render pipline
  ///
  /// This forces all subsequent render draw calls to render into this FBO.
  /// @note Do not call this function directly, use `GLRenderer::bindFrameBufferObject` instead
  void bind();

  /// @brief Unbind the frame buffer object
  static void unbind();

  /// @brief Convert to string
  virtual std::string toString() const override;

  SEQUOIA_GL_OBJECT(FrameBufferObject)
};

} // namespace render

} // namespace sequoia

#endif
