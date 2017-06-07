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

#include "sequoia/Core/Casting.h"
#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLProgram.h"
#include "sequoia/Render/GL/GLStateCacheManager.h"
#include "sequoia/Render/GL/GLVertexArrayObject.h"
#include "sequoia/Render/GL/GLVertexAttribute.h"
#include "sequoia/Render/VertexData.h"

namespace sequoia {

namespace render {

namespace {

static GLenum getGLDrawMode(VertexData::DrawModeKind mode) {
  switch(mode) {
  case VertexData::DrawModeKind::DM_Triangles:
    return GL_TRIANGLES;
  default:
    sequoia_unreachable("invalid DrawModeKind");
  }
}

template <class T>
struct GetIndexType {};

template <>
struct GetIndexType<unsigned int> {
  static constexpr GLenum value = GL_UNSIGNED_INT;
};

} // anonymous namespace

GLStateCacheManager::GLStateCacheManager() {
  program_ = nullptr;
  vao_ = nullptr;
}

void GLStateCacheManager::setRenderState(const sequoia::render::RenderState& renderState) {
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
}

void GLStateCacheManager::setProgram(GLProgram* program) {
  if(program_ != program) {
    program_ = program;
    program_->bind();
  }
}

void GLStateCacheManager::draw(DrawCommand* command) {
  GLVertexArrayObject* vao = dyn_cast<GLVertexArrayObject>(command->getVertexArrayObject());

  if(vao != vao_) {
    vao_ = vao;
    vao_->bind();
  }

  if(vao_->hasIndices()) {
    glDrawElements(getGLDrawMode(vao_->getVertexData()->getDrawMode()), vao_->getNumIndices(),
                   GetIndexType<VertexIndexType>::value, (void*)0);
  } else {
    glDrawArrays(getGLDrawMode(vao_->getVertexData()->getDrawMode()), 0, vao_->getNumVertices());
  }
}

} // namespace render

} // namespace sequoia
