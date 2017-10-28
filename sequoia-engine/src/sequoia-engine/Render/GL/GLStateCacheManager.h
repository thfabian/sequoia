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

#ifndef SEQUOIA_RENDER_GL_GLSTATECACHEMANAGER_H
#define SEQUOIA_RENDER_GL_GLSTATECACHEMANAGER_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/NonCopyable.h"
#include "sequoia-engine/Render/FrameListener.h"
#include "sequoia-engine/Render/GL/GLFwd.h"
#include "sequoia-engine/Render/RenderFwd.h"
#include <memory>
#include <unordered_map>

namespace sequoia {

namespace render {

class GLRenderStateCache;

/// @brief Pixel storage modes
/// @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glPixelStore.xhtml
class SEQUOIA_API GLPixelFormat {
  friend class GLRenderStateCache;

  /// Map of the the parameter of the pixel format to their respective values
  std::unordered_map<GLenum, int> format_;

public:
  /// @brief Get the value of `param`
  int get(GLenum param) const noexcept;

  /// @brief Set the `value` of `param`
  void set(GLenum param, int value) noexcept;

  /// @brief Convert pixel format to string
  std::string toString() const;
};

/// @brief Manager of the OpenGL state-machine
///
/// This keeps an in-memory copy of the OpenGL state-machine to avoid unnecessary state changes.
///
/// @ingroup gl
class SEQUOIA_API GLStateCacheManager : public FrameListener, public NonCopyable {

  /// In-memory version of the OpenGL state
  std::shared_ptr<GLRenderStateCache> stateCache_;

  /// Default pixel format
  GLPixelFormat defaultPixelFormat_;

public:
  GLStateCacheManager();

  /// @brief Return the value of `param` of type `T`
  ///
  /// Type conversion is performed if `param` has a different type than the state variable value
  /// being requested using standard C-type conversion.
  template <class T>
  T get(GLenum param) const noexcept {
    return getImpl(param, T());
  }

  /// @brief Draw the `command` and update the RenderState *if* necessary
  /// @returns `true` if the command was successfully drawn, `false` otherwise
  bool draw(DrawCommand* command);

  /// @brief Set the uniform variable `name` of `program` to `value`
  ///
  /// @param program   Program to set the uniform variable
  /// @param name      Name of the uniform variable
  /// @param value     Uniform variable to set
  /// @returns `true` if the variable was successfully set, `false` if the program is not valid
  bool setUniformVariable(Program* program, const std::string& name, const UniformVariable& value);

  /// @brief Get the currently active RenderState
  const RenderState& getRenderState() const;

  /// @brief Bind the `program`
  void bindProgram(Program* program);

  /// @brief Unbind any programs
  void unbindProgram();

  /// @brief Bind the given vertex data
  void bindVertexDataForModify(VertexData* data);

  /// @brief Bind the given vertex data
  void bindVertexDataForDrawing(VertexData* data);

  /// @brief Unbind any vertex array objects
  void unbindVertexData();

  /// @brief Bind the given frame buffer object `fbo`
  void bindFrameBufferObject(FrameBuffer* fbo);

  /// @brief Unbind any frame buffer objects
  void unbindFrameBufferObject();

  /// @brief Bind the `texture` to `textureUnit`
  void bindTexture(int textureUnit, Texture* texture, bool force = false);

  /// @brief Unbind any texture from `textureUnit`
  void unbindTexture(int textureUnit);

  /// @brief Set the viewport geometry
  void setViewport(int x, int y, int width, int height);
  void setViewport(Viewport* viewport);

  /// @brief Get the default `GLPixelFormat`
  GLPixelFormat getDefaultPixelFormat() const noexcept { return defaultPixelFormat_; }

  /// @brief Set the pixel storage mode to `format`
  ///
  /// This affects all subsequent calls to `glReadPixels` as well as the unpacking of textures (e.g
  /// `glTexImage2D`).
  void setPixelFormat(const GLPixelFormat& format);

  /// @brief Reset the pixel storage format mode to it's default
  void resetPixelFormat();

  void frameListenerRenderingBegin(RenderCommand* command) override;
  void frameListenerRenderingEnd(RenderCommand* command) override;

protected:
  bool getImpl(GLenum param, bool) const noexcept;
  int getImpl(GLenum param, int) const noexcept;
  float getImpl(GLenum param, float) const noexcept;
  double getImpl(GLenum param, double) const noexcept;
};

} // namespace render

} // namespace sequoia

#endif
