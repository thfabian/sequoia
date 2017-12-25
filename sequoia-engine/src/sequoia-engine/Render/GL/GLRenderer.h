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

#ifndef SEQUOIA_ENGINE_RENDER_GL_GLRENDERER_H
#define SEQUOIA_ENGINE_RENDER_GL_GLRENDERER_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/NonCopyable.h"
#include "sequoia-engine/Core/Options.h"
#include "sequoia-engine/Render/GL/GLBuffer.h"
#include "sequoia-engine/Render/GL/GLFwd.h"
#include "sequoia-engine/Render/GL/GLPixelFormat.h"
#include "sequoia-engine/Render/RenderFwd.h"
#include "sequoia-engine/Render/Renderer.h"
#include "sequoia-engine/Render/Viewport.h"
#include <memory>

namespace sequoia {

namespace render {

/// @brief OpenGL based renderer
///
/// A rendered performs the actual rendering of the provided target. Further, the renderer keeps
/// track on the OpenGL state machine.
///
/// @note Any other object which needs to temporarly modify the OpenGL state machine needs to make
/// sure to undo the modification after being done. It is highly recommended to use DSA
/// (direct-state-access) outside of the Renderer.
///
/// @ingroup gl
class SEQUOIA_API GLRenderer final : public Renderer, public ViewportListener, public NonCopyable {
  /// Reference to the main-window
  GLRenderWindow* window_;

  /// Is debug mod enabled?
  bool debugMode_;

  /// Managers
  std::unique_ptr<GLShaderManager> shaderManager_;
  std::unique_ptr<GLProgramManager> programManager_;
  std::unique_ptr<GLTextureManager> textureManager_;
  std::unique_ptr<GLExtensionManager> extensionManager_;

  /// OpenGL specific state
  int activeTextureUnit_;
  GLPixelFormat pixelFormat_;

  /// Default pixel format
  GLPixelFormat defaultPixelFormat_;

public:
  /// @brief Initialize the OpenGL context and bind it to the calling thread
  ///
  /// This also registers the renderer as a viewport listener of `window`.
  GLRenderer(GLRenderWindow* window, Options& options);

  /// @brief Release the OpenGL context
  ~GLRenderer();

  /// @copydoc Renderer::reset()
  virtual void reset() override;

  /// @brief Get the shader manager
  GLShaderManager* getShaderManager();

  /// @brief Get the program manager
  GLProgramManager* getProgramManager();

  /// @brief Get the texture manager
  GLTextureManager* getTextureManager();

  /// @brief Return the value of `param` of type `T`
  ///
  /// Type conversion is performed if `param` has a different type than the state variable value
  /// being requested using standard C-type conversion.
  template <class T>
  T get(GLenum param) const noexcept {
    return getImpl(param, T());
  }

  /// @brief Set a texture-unit/texture pair
  bool setTexture(int textureUnit, Texture* texture);

  /// @brief Unset all texture-unit/texture pairs
  ///
  /// This effectivly disables all texture units
  bool unsetTextures();

  /// @brief Get the default `GLPixelFormat`
  GLPixelFormat getDefaultPixelFormat() const noexcept { return defaultPixelFormat_; }

  /// @brief Set the pixel storage mode to `format`
  ///
  /// This affects all subsequent calls to `glReadPixels` as well as the unpacking of textures (e.g
  /// `glTexImage2D`).
  void setPixelFormat(const GLPixelFormat& format, bool force = false);

  /// @brief Reset the pixel storage format mode to it's default
  void resetPixelFormat();

  /// @brief Check if `extension` is supported
  /// @see GLExtensionManager::supported
  bool isExtensionSupported(gl::GLextension extension) noexcept;

  void viewportGeometryChanged(Viewport* viewport) override;

private:
  bool getImpl(GLenum param, bool) const noexcept;
  int getImpl(GLenum param, int) const noexcept;
  float getImpl(GLenum param, float) const noexcept;
  double getImpl(GLenum param, double) const noexcept;

  /// @brief Set the active texture unit
  inline void setActiveTextureUnit(int textureUnit) noexcept;

protected:
  using Base = Renderer;

#define RENDER_STATE(Type, Name, DefaultValue) virtual bool Name##Changed(Type value) override;
#include "sequoia-engine/Render/RenderState.inc"
#undef RENDER_STATE

  /// @copydoc Renderer::ProgramChanged
  virtual bool ProgramChanged(Program* program) override;

  /// @copydoc Renderer::VertexDataChanged
  virtual bool VertexDataChanged(VertexData* data) override;

  /// @copydoc Renderer::TextureChanged
  virtual bool TextureChanged(int textureUnit, Texture* texture, bool enable) override;

  /// @copydoc Renderer::UniformVariableChanged
  virtual bool UniformVariableChanged(Program* program, const std::string& name,
                                      const UniformVariable& value) override;

  /// @copydoc Renderer::ViewportChanged
  virtual bool ViewportChanged(int x, int y, int width, int height) override;

  /// @copydoc Renderer::clearRenderBuffers
  virtual bool
  clearRenderBuffers(const std::set<RenderBuffer::RenderBufferKind>& buffersToClear) override;

  /// @copydoc Renderer::draw
  virtual bool draw(const DrawCommand& drawCommand) override;

  /// @copydoc Renderer::toStringImpl
  std::pair<std::string, std::string> toStringImpl() const override;
};

// TODO: this doesn't play nicely with multiple contexts
/// @brief Get a reference to the currently active `GLRenderer`
/// @ingroup gl
/// @{
SEQUOIA_API extern GLRenderer& getGLRenderer() noexcept;
SEQUOIA_API extern GLRenderer* getGLRendererPtr() noexcept;
/// @}

} // namespace render

} // namespace sequoia

#endif
