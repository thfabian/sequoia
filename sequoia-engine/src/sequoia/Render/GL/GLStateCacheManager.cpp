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

#include "sequoia/Render/GL/GL.h"
#include "sequoia/Core/Casting.h"
#include "sequoia/Render/GL/GLProgram.h"
#include "sequoia/Render/GL/GLStateCacheManager.h"
#include "sequoia/Render/GL/GLVertexArrayObject.h"
#include "sequoia/Render/GL/GLVertexAttribute.h"
#include "sequoia/Render/VertexData.h"

namespace sequoia {

namespace render {

namespace {

/// @brief Get the `GLenum` of the draw-mode
static GLenum getGLDrawMode(VertexData::DrawModeKind mode) {
  switch(mode) {
  case VertexData::DrawModeKind::DM_Triangles:
    return GL_TRIANGLES;
  default:
    sequoia_unreachable("invalid DrawModeKind");
  }
}

/// @brief Get the `GLenum` of the index type
template <class T>
struct GetIndexType {};

template <>
struct GetIndexType<unsigned int> {
  static constexpr GLenum value = GL_UNSIGNED_INT;
};

/// @brief OpenGL implementation of the RenderStateCache
/// @ingroup gl
class GLRenderStateCache : public RenderStateCache {
public:
  GLRenderStateCache() : RenderStateCache() { initState(); }

protected:
  /// @brief Enable/Disable the capability `cap`
  inline void setCapability(bool enable, GLenum cap) noexcept {
    if(enable)
      glEnable(cap);
    else
      glDisable(cap);
  }

  virtual void DepthTestChanged(bool DepthTest) override {
    setCapability(DepthTest, GL_DEPTH_TEST);
  }

  virtual void DepthFuncChanged(RenderState::DepthFuncKind DepthFunc) override {
    switch(DepthFunc) {
    case RenderState::DepthFuncKind::DF_Never:
      glDepthFunc(GL_NEVER);
      break;
    case RenderState::DepthFuncKind::DF_Less:
      glDepthFunc(GL_LESS);
      break;
    case RenderState::DepthFuncKind::DF_Equal:
      glDepthFunc(GL_LESS);
      break;
    case RenderState::DepthFuncKind::DF_LessEqual:
      glDepthFunc(GL_LEQUAL);
      break;
    case RenderState::DepthFuncKind::DF_Greater:
      glDepthFunc(GL_GREATER);
      break;
    case RenderState::DepthFuncKind::DF_NotEqual:
      glDepthFunc(GL_NOTEQUAL);
      break;
    case RenderState::DepthFuncKind::DF_GreaterEqual:
      glDepthFunc(GL_GEQUAL);
      break;
    case RenderState::DepthFuncKind::DF_Always:
      glDepthFunc(GL_ALWAYS);
      break;
    default:
      sequoia_unreachable("invalid DepthFuncKind");
    }
  }

  virtual void ProgramChanged(Program* program) override {
    if(program)
      dyn_cast<GLProgram>(program)->bind();
  }

  virtual void VertexArrayObjectChanged(VertexArrayObject* vao) override {
    if(vao)
      dyn_cast<GLVertexArrayObject>(vao)->bind();
  }
};

} // anonymous namespace

GLStateCacheManager::GLStateCacheManager() { stateCache_ = std::make_unique<GLRenderStateCache>(); }

void GLStateCacheManager::draw(DrawCommand* command) {
  // Update the render-state
  stateCache_->setRenderState(command->getRenderState());
  const RenderState& state = getRenderState();

  // Draw the VAO
  GLVertexArrayObject* vao = dyn_cast<GLVertexArrayObject>(state.VertexArrayObject);

  if(vao->hasIndices()) {
    glDrawElements(getGLDrawMode(vao->getVertexData()->getDrawMode()), vao->getNumIndices(),
                   GetIndexType<VertexIndexType>::value, (void*)0);
  } else {
    glDrawArrays(getGLDrawMode(vao->getVertexData()->getDrawMode()), 0, vao->getNumVertices());
  }
}

const RenderState& GLStateCacheManager::getRenderState() const {
  return stateCache_->getRenderState();
}

} // namespace render

} // namespace sequoia
