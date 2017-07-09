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

#include "sequoia/Core/Casting.h"
#include "sequoia/Render/DrawCommand.h"
#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLFrameBufferObject.h"
#include "sequoia/Render/GL/GLProgram.h"
#include "sequoia/Render/GL/GLStateCacheManager.h"
#include "sequoia/Render/GL/GLTexture.h"
#include "sequoia/Render/GL/GLVertexArrayObject.h"
#include "sequoia/Render/GL/GLVertexAttribute.h"
#include "sequoia/Render/RenderState.h"
#include "sequoia/Render/VertexData.h"
#include "sequoia/Render/Viewport.h"

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

/// @brief Get the enum of the index type
template <class T>
struct GetIndexType {};

template <>
struct GetIndexType<unsigned int> {
  static constexpr GLenum value = GL_UNSIGNED_INT;
};

} // anonymous namespace

/// @brief OpenGL implementation of the RenderStateCache
/// @ingroup gl
class GLRenderStateCache final : public RenderStateCache {

  /// Keep track of the values of the uniform variables of the Programs
  std::unordered_map<unsigned int, std::unordered_map<std::string, UniformVariable>>
      programUniformMap_;

  /// Currently active texture unit
  int activeTextureUnit_;

  /// Viewport
  int x_, y_, width_, height_;

public:
  GLRenderStateCache()
      : RenderStateCache(), activeTextureUnit_(-1), x_(-1), y_(-1), width_(-1), height_(-1) {
    initState();
  }

  /// @brief Set the viewport of the scene
  void setViewport(int x, int y, int width, int height) {
    if(x_ != x || y_ != y || width_ != width || height_ != height)
      glViewport(x, y, width, height);
  }

  /// @brief Bind the `program`
  void bindProgram(Program* program) {
    if(getRenderState().Program != program)
      ProgramChanged(program);
  }

  /// @brief Unbind any `program`
  void unbindProgram() { GLProgram::unbind(); }

  /// @brief Bind the given `VBO`
  void bindVertexArrayObject(VertexArrayObject* vao) {
    if(getRenderState().VertexArrayObject != vao)
      VertexArrayObjectChanged(vao);
  }

  /// @brief Unbind any `VBO`
  void unbindVertexArrayObject() { GLVertexArrayObject::unbind(); }

  /// @brief Bind the given `FBO`
  void bindFrameBufferObject(FrameBufferObject* fbo) {
    if(fbo)
      dyn_cast<GLFrameBufferObject>(fbo)->bind();
  }

  /// @brief Unbind any `FBO`
  void unbindFrameBufferObject() { GLFrameBufferObject::unbind(); }

  /// @brief Bind the `texture` to `textureUnit`
  void bindTexture(int textureUnit, Texture* texture, bool force) {
    if(!force) {
      // Only bind the texture if it is not already bound
      if(!getRenderState().TextureMap.count(textureUnit) ||
         getRenderState().TextureMap.find(textureUnit)->second != texture)
        TextureChanged(textureUnit, texture, true);
    } else {
      // Force the binding
      setActiveTextureUnit(textureUnit);
      getRenderState().TextureMap[textureUnit] = texture;
      dyn_cast<GLTexture>(texture)->bind();
    }
  }

  /// @brief Unbind any `texture` from `textureUnit`
  void unbindTexture(int textureUnit) {
    auto it = getRenderState().TextureMap.find(textureUnit);

    if(it != getRenderState().TextureMap.end()) {
      setActiveTextureUnit(textureUnit);
      dyn_cast<GLTexture>(it->second)->unbind();
      getRenderState().TextureMap.erase(it);
    }
  }

  /// @brief Update the uniform variables of `program` if necessary
  void
  setUniformVariables(Program* program,
                      const std::unordered_map<std::string, UniformVariable>& newUniformVariables) {
    GLProgram* glProgram = dyn_cast<GLProgram>(program);

    // Only update the variables if they differ
    for(const auto& nameVariablePair : newUniformVariables) {
      setUniformVariable(glProgram, nameVariablePair.first, nameVariablePair.second);
    }
  }

  /// @brief Update the uniform variable of `glProgram` if necessary
  inline void setUniformVariable(GLProgram* glProgram, const std::string& name,
                                 const UniformVariable& variable) {

    std::unordered_map<std::string, UniformVariable>& oldUniformVariable =
        programUniformMap_[glProgram->getID()];

    auto it = oldUniformVariable.find(name);
    bool updateNeeded = false;
    if(it == oldUniformVariable.end()) {
      // Variable does not exist -> update
      updateNeeded = true;
    } else {
      if(it->second != variable)
        // Variable exists but with different value -> update
        updateNeeded = true;
    }

    if(updateNeeded) {
      glProgram->setUniformVariable(name, variable);
      oldUniformVariable[name] = variable;
    }
  }

  /// @brief Reset the uniform variables
  void resetUniformVariables() { programUniformMap_.clear(); }

protected:
  /// @brief Enable/Disable the capability `cap`
  inline void setCapability(bool enable, GLenum cap) noexcept {
    if(enable)
      glEnable(cap);
    else
      glDisable(cap);
  }

  /// @brief Set the active texture unit
  inline void setActiveTextureUnit(int textureUnit) noexcept {
    if(activeTextureUnit_ != textureUnit) {
      glActiveTexture(GL_TEXTURE0 + textureUnit);
      activeTextureUnit_ = textureUnit;
    }
  }

  virtual bool DepthTestChanged(bool DepthTest) override {
    setCapability(DepthTest, GL_DEPTH_TEST);
    return true;
  }

  virtual bool DepthFuncChanged(RenderState::DepthFuncKind DepthFunc) override {
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
    return true;
  }

  virtual bool ProgramChanged(Program* program) override {
    SEQUOIA_ASSERT(program);

    GLProgram* glprogram = dyn_cast<GLProgram>(program);
    if(!glprogram->isValid())
      return false;

    glprogram->bind();
    return true;
  }

  virtual bool VertexArrayObjectChanged(VertexArrayObject* vao) override {
    SEQUOIA_ASSERT(vao);

    GLVertexArrayObject* glvao = dyn_cast<GLVertexArrayObject>(vao);

    glvao->bind();
    return true;
  }

  virtual bool TextureChanged(int textureUnit, Texture* texture, bool enable) override {
    SEQUOIA_ASSERT(texture);

    GLTexture* gltexture = dyn_cast<GLTexture>(texture);

    // Bind texture
    setActiveTextureUnit(textureUnit);

    if(enable) {
      if(!gltexture->isValid())
        return false;

      gltexture->bind();

      // Bind assoicated sampler if we already have a program
      Program* program = getRenderState().Program;
      if(program) {
        GLProgram* glProgram = dyn_cast<GLProgram>(program);
        const std::string& name = glProgram->getTextureSampler(textureUnit);
        setUniformVariable(glProgram, name, UniformVariable(textureUnit));
      }
    } else {
      dyn_cast<GLTexture>(texture)->unbind();
    }

    return true;
  }
};

GLStateCacheManager::GLStateCacheManager() { stateCache_ = std::make_unique<GLRenderStateCache>(); }

bool GLStateCacheManager::draw(DrawCommand* command) {
  if(!command->getProgram()->isValid())
    return false;

  // Set the uniform variables of the program
  if(!command->getUniformVariables().empty())
    stateCache_->setUniformVariables(command->getProgram(), command->getUniformVariables());

  // Update the render-state (including setting the VAO, program, textures etc..)
  if(!stateCache_->setRenderState(command->getRenderState()))
    return false;

  const RenderState& state = getRenderState();

  // Check that all uniform variables are set correctly
  dyn_cast<GLProgram>(state.Program)->checkUniformVariables();

  // Draw the VAO
  GLVertexArrayObject* vao = dyn_cast<GLVertexArrayObject>(state.VertexArrayObject);
  if(vao->hasIndices()) {
    glDrawElements(getGLDrawMode(vao->getVertexData()->getDrawMode()), vao->getNumIndices(),
                   GetIndexType<VertexIndexType>::value, (void*)0);
  } else {
    glDrawArrays(getGLDrawMode(vao->getVertexData()->getDrawMode()), 0, vao->getNumVertices());
  }

  return true;
}

const RenderState& GLStateCacheManager::getRenderState() const {
  return stateCache_->getRenderState();
}

void GLStateCacheManager::bindProgram(Program* program) { stateCache_->bindProgram(program); }

void GLStateCacheManager::unbindProgram() { stateCache_->unbindProgram(); }

void GLStateCacheManager::bindVertexArrayObject(VertexArrayObject* vao) {
  stateCache_->bindVertexArrayObject(vao);
}

void GLStateCacheManager::unbindVertexArrayObject() { stateCache_->unbindVertexArrayObject(); }

void GLStateCacheManager::bindFrameBufferObject(FrameBufferObject* fbo) {
  stateCache_->bindFrameBufferObject(fbo);
}

void GLStateCacheManager::unbindFrameBufferObject() { stateCache_->unbindFrameBufferObject(); }

void GLStateCacheManager::bindTexture(int textureUnit, Texture* texture, bool force) {
  stateCache_->bindTexture(textureUnit, texture, force);
}

void GLStateCacheManager::unbindTexture(int textureUnit) {
  stateCache_->unbindTexture(textureUnit);
}

void GLStateCacheManager::setViewport(int x, int y, int width, int height) {
  stateCache_->setViewport(x, y, width, height);
}

void GLStateCacheManager::setViewport(Viewport* viewport) {
  stateCache_->setViewport(viewport->getX(), viewport->getY(), viewport->getWidth(),
                           viewport->getHeight());
}

void GLStateCacheManager::frameListenerRenderingBegin(RenderTarget* target) {
  stateCache_->resetUniformVariables();
}

void GLStateCacheManager::frameListenerRenderingEnd(RenderTarget* target) {}

bool GLStateCacheManager::getImpl(GLenum param, bool) const noexcept {
  GLboolean value;
  glGetBooleanv(param, &value);
  return static_cast<bool>(value);
}

int GLStateCacheManager::getImpl(GLenum param, int) const noexcept {
  int value;
  glGetIntegerv(param, &value);
  return value;
}

float GLStateCacheManager::getImpl(GLenum param, float) const noexcept {
  float value;
  glGetFloatv(param, &value);
  return value;
}

double GLStateCacheManager::getImpl(GLenum param, double) const noexcept {
  double value;
  glGetDoublev(param, &value);
  return value;
}

} // namespace render

} // namespace sequoia
