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

#include "sequoia-engine/Render/FrameBuffer.h"
#include "sequoia-engine/Render/GL/GLFragmentData.h"
#include <type_traits>
#include <unordered_map>

namespace sequoia {

namespace render {

class GLTexture;

/// @brief OpenGL implemenation of the frame-buffer objects
/// @ingroup gl
class SEQUOIA_API GLFrameBufferObject final : public FrameBuffer {

  /// OpenGL index of the frame buffer
  unsigned int fboID_;

  /// OpenGL index of the depth render buffer
  unsigned int rboDepthID_;

  /// OpenGL index of the color render buffer
  unsigned int rboColorID_;

  /// Parameter used for initialization
  FrameBufferParameter param_;

  /// Textures bound to the correspond output fragment data, see `GLFragmentData::Data`
  std::unordered_map<unsigned int, std::shared_ptr<GLTexture>> fragmentDataMap_;

public:
  GLFrameBufferObject(const FrameBufferParameter& param);
  virtual ~GLFrameBufferObject();

  /// @brief Get the unique identifer of the program
  ///
  /// Note that IDs might be reused after a buffer has been destroyed.
  unsigned int getID() const;

  /// @brief Get the parameter used for initialization
  const FrameBufferParameter& getParam() const;

  /// @brief Attach a `texture` to the frame-buffer which captures the output of the
  /// `attachmentPoint`
  ///
  /// Note that the size of the texture needs to be consistent with the size of the framebuffer as
  /// supplied by the FrameBufferObjectParameter in the constructor.
  void attachTexture(GLFragmentData::Data attachmentPoint,
                     const std::shared_ptr<GLTexture>& texture);

  /// @brief Bind the frame buffer object to the current render pipline
  ///
  /// This forces all subsequent draw calls to render into this FBO.
  /// @note Do not call this function directly, use `GLRenderer::bindFrameBufferObject` instead
  void bind();

  /// @brief Unbind any frame buffer object
  static void unbind();

  /// @brief Convert to string
  virtual std::string toString() const override;

  SEQUOIA_GL_OBJECT(FrameBuffer)

private:
  std::vector<GLenum> getDrawBuffers() const noexcept;
};

} // namespace render

} // namespace sequoia

#endif
