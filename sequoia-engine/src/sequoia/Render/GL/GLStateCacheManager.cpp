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
#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLProgram.h"
#include "sequoia/Render/GL/GLStateCacheManager.h"
#include "sequoia/Render/GL/GLVertexArrayObject.h"
#include "sequoia/Render/GL/GLVertexAttribute.h"
#include "sequoia/Render/VertexData.h"

#include <iostream>

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

} // anonymous namespace

/// @brief OpenGL implementation of the RenderStateCache
/// @ingroup gl
class GLRenderStateCache : public RenderStateCache {

  /// Keep track of the values of the uniform variables of the Programs
  std::unordered_map<unsigned int, std::unordered_map<std::string, UniformVariable>>
      programUniformMap_;

public:
  GLRenderStateCache() : RenderStateCache() { initState(); }

  /// @brief Bind the `program`
  void bindProgram(Program* program) {
    if(getRenderState().Program != program)
      ProgramChanged(program);
  }

  /// @brief Bind the given `VBO`
  void bindVertexArrayObject(VertexArrayObject* vao) {
    if(getRenderState().VertexArrayObject != vao)
      VertexArrayObjectChanged(vao);
  }

  /// @brief Update the uniform variables of a program if necessary
  void
  setUniformVariables(Program* program,
                      const std::unordered_map<std::string, UniformVariable>& newUniformVariables) {
    GLProgram* glProgram = dyn_cast<GLProgram>(program);

    // We only update the variable if we *have* to!
    std::unordered_map<std::string, UniformVariable>& oldUniformVariable =
        programUniformMap_[glProgram->getID()];

    // Program has already been processed, only update the variables if they differ
    for(const auto& nameVariablePair : newUniformVariables) {
      const std::string& name = nameVariablePair.first;
      const UniformVariable& variable = nameVariablePair.second;

      auto it = oldUniformVariable.find(name);
      bool updateNeeded = false;
      if(it == oldUniformVariable.end()) {
        updateNeeded = true;
      } else {
        if(it->second != variable)
          updateNeeded = true;
      }

      if(updateNeeded) {
        // std::cout << name << "\n" << variable.toString() << std::endl;
        glProgram->setUniformVariable(name, variable);
        oldUniformVariable[name] = variable;
      }
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

GLStateCacheManager::GLStateCacheManager() { stateCache_ = std::make_unique<GLRenderStateCache>(); }

void GLStateCacheManager::draw(DrawCommand* command) {

  // Set the uniform variables if necessary
  if(!command->getUniformVariables().empty())
    stateCache_->setUniformVariables(command->getProgram(), command->getUniformVariables());

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

void GLStateCacheManager::bindProgram(Program* program) { stateCache_->bindProgram(program); }

void GLStateCacheManager::bindVertexArrayObject(VertexArrayObject* vao) {
  stateCache_->bindVertexArrayObject(vao);
}

void GLStateCacheManager::startRendering() { stateCache_->resetUniformVariables(); }

} // namespace render

} // namespace sequoia
