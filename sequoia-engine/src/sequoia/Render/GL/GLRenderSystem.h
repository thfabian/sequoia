//===--------------------------------------------------------------------------------*- C++ -*-===//
//                         _____                        _
//                        / ____|                      (_)
//                       | (___   ___  __ _ _   _  ___  _  __ _
//                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
//                        ____) |  __/ (_| | |_| | (_) | | (_| |
//                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine
//                                       | |
//                                       |_|
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#ifndef SEQUOIA_RENDER_GL_GLRENDERSYSTEM_H
#define SEQUOIA_RENDER_GL_GLRENDERSYSTEM_H

#include "sequoia/Core/Singleton.h"
#include "sequoia/Render/GL/GLRenderWindow.h"
#include "sequoia/Render/RenderData.h"
#include "sequoia/Render/RenderSystemImpl.h"
#include <memory>
#include <unordered_map>
#include <vector>

namespace sequoia {

namespace render {

/// @brief OpenGL render-system
/// @ingroup render
class SEQUOIA_RENDER_API GLRenderSystem : public RenderSystemImpl {
  std::vector<RenderData*> renderData_;
  std::unordered_map<int, std::shared_ptr<GLRenderWindow>> renderWindows_;

public:
  /// @brief Initialize GLFW
  /// @throws RenderSystemInitException    Initialization of GLFW failed
  GLRenderSystem();

  /// @brief Terminates GLFW
  virtual ~GLRenderSystem();

  /// @copydoc RenderSystemImpl::createWindow
  virtual int createWindow(const std::string& title) override;

  /// @copydoc RenderSystemImpl::getWindow
  virtual RenderWindow* getWindow(int windowID) override;

  /// @copydoc RenderSystemImpl::pollEvents
  virtual void pollEvents() override;
};

} // namespace render

} // namespace sequoia

#endif
