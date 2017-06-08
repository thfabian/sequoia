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

#include "sequoia/Core/Format.h"
#include "sequoia/Core/StringUtil.h"
#include "sequoia/Render/DrawCommand.h"
#include "sequoia/Render/Program.h"
#include "sequoia/Render/VertexArrayObject.h"

namespace sequoia {

namespace render {

std::string DrawCommand::toString() const {
  return core::format("DrawCommand["
                      "  program = %s,\n"
                      "  vao = %s,\n"
                      "  renderState = %s,\n"
                      "  modelMatrix = %s\n"
                      "]",
                      program_->toString(), vao_->toString(), core::indent(state_.toString()),
                      modelMatrix_);
}

} // namespace render

} // namespace sequoia
