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

#ifndef SEQUOIA_RENDER_GL_NATIVE_H
#define SEQUOIA_RENDER_GL_NATIVE_H

#include "sequoia/Core/Export.h"
#include "sequoia/Core/Listenable.h"
#include "sequoia/Core/Options.h"
#include "sequoia/Render/Input.h"
#include "sequoia/Render/RenderWindow.h"
#include <memory>

namespace sequoia {

namespace render {

class NativeWindow;
class NativeGLContext;
class NativeInputSystem;

/// @brief Kind of window-system
enum class NativeWindowSystemKind {
  NK_GLFW3 ///< glfw3 based window-system
};

/// @brief Native OpenGL context
/// @ingroup gl
class SEQUOIA_API NativeGLContext {
public:
  /// @brief Create context (to initialize the context, call `NativeContext::init()`)
  NativeGLContext(NativeWindowSystemKind kind) : kind_(kind) {}

  /// @brief Virtual destructor
  virtual ~NativeGLContext() {}

  /// @brief Use the `hints` and `Options` to construct the native context
  ///
  /// @param windowHints    Window hints used to construct the context (and window)
  ///
  /// This may also create a window for window systems which tie the context to the window like
  /// glfw3.
  virtual void init(const RenderWindow::WindowHint& windowHints, Options* options) = 0;

  /// @brief Construct a context from an existing `context`
  ///
  /// @param context    Parent context
  ///
  /// The new context should be coupled to `context` e.g share all the buffers, textures, shaders
  /// and programs.
  virtual void init(const std::shared_ptr<NativeGLContext>& context) = 0;

  /// @brief Makes the context current for the calling thread
  virtual void makeCurrent() = 0;

  /// @brief Enable vertical synchronization i.e wait for atleast one screen update before the
  /// buffers are swapped
  virtual void enableVSync() = 0;

  /// @brief Get the kind of system
  NativeWindowSystemKind getKind() const { return kind_; }

  /// @brief Create a new OpenGL context using the given window-system `kind`
  ///
  /// @note To initialize the context, call `NativeGLContext::init()`
  static std::shared_ptr<NativeGLContext> create(NativeWindowSystemKind kind);

private:
  NativeWindowSystemKind kind_;
};

/// @brief Listen to changes of the window (resized, focused gained etc.)
/// @ingroup gl
class SEQUOIA_API NativeWindowListener {
public:
  virtual ~NativeWindowListener() {}

  /// @brief Window has been resized
  virtual void nativeWindowGeometryChanged(NativeWindow* window) = 0;

  /// @brief Window has taken focus
  virtual void nativeWindowFocusChanged(NativeWindow* window) = 0;
};

/// @brief Native window
/// @ingroup gl
class SEQUOIA_API NativeWindow : public Listenable<NativeWindowListener> {
public:
  /// @brief Virtual destructor
  virtual ~NativeWindow() {}

  /// @brief Swaps the frame buffers to display the next frame
  virtual void swapBuffers() = 0;

  /// @brief Check if window is hidden
  virtual bool isHidden() = 0;

  /// @brief Check if window is closed
  virtual bool isClosed() = 0;

  /// @brief Check if window is in fullscreen mode
  virtual bool isFullscreen() const = 0;

  /// @brief Check if window is in focus
  virtual bool isFocused() const = 0;

  /// @brief Get the current width of the window
  virtual int getWidth() const = 0;

  /// @brief Get the current height of the window
  virtual int getHeight() const = 0;

  /// @brief Get the associated context
  virtual NativeGLContext* getContext() const = 0;

  /// @brief Create a new native window using `context`
  static std::shared_ptr<NativeWindow> create(const std::shared_ptr<NativeGLContext>& context);
};

/// @brief Native input system
///
/// An input system is attached to a specific window. Usually only one input system can be
/// registered at the time.
///
/// @ingroup gl
class SEQUOIA_API NativeInputSystem : public Listenable<KeyboardListener, MouseListener> {
public:
  /// @brief Virtual destructor
  virtual ~NativeInputSystem() {}

  /// @brief Poll the input events
  virtual void pollEvents() = 0;

  /// @brief Center the cursor
  virtual void centerCursor() = 0;

  /// @brief Set the mode of the cursor
  virtual void setCursorMode(RenderWindow::CursorModeKind mode) = 0;

  /// @brief Get the mode of the cursor
  virtual RenderWindow::CursorModeKind getCursorMode() = 0;

  /// @brief Set mouse position to `(xpos, ypos)`
  virtual void setCursorPosition(int xpos, int ypos) = 0;

  /// @brief Get the associated window
  virtual NativeWindow* getWindow() const = 0;

  /// @brief Attach an input system to the `window`
  static std::shared_ptr<NativeInputSystem> create(const std::shared_ptr<NativeWindow>& window,
                                                   bool centerCursor);
};

} // render

} // namespace sequoia

#endif
