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

#include "sequoia/Render/GL/GLProgram.h"

namespace sequoia {

namespace render {

GLProgram::GLProgram(const std::set<Shader*>& shaders)
    : Program(RK_OpenGL), id_(0), shaders_(shaders) {}

const std::set<Shader*>& GLProgram::getShaders() const { return shaders_; }

unsigned int GLProgram::getID() const { return id_; }

std::string GLProgram::toString() const { return std::string(); }

} // namespace render

} // namespace sequoia
