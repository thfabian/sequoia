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

#ifndef SEQUOIA_RENDER_GL_GLPROGRAM_H
#define SEQUOIA_RENDER_GL_GLPROGRAM_H

#include "sequoia/Render/Program.h"

namespace sequoia {

namespace render {

class GLProgramManager;

/// @brief OpenGL implementation of a GPUProgram
/// @see RenderSystem::createPogram
/// @ingroup gl
class SEQUOIA_RENDER_API GLProgram : public Program {

  /// OpenGL program index
  unsigned int id_;

  /// Shaders compiled into the program
  std::set<Shader*> shaders_;

public:
  /// @brief Create the program (this tries to make every shader valid)
  GLProgram(const std::set<Shader*>& shaders);

  /// @copydoc Program::getID()
  virtual unsigned int getID() const override;

  /// @copydoc Program::toString()
  virtual std::string toString() const override;

  /// @copydoc Program::getShaders()
  virtual const std::set<Shader*>& getShaders() const override;

  SEQUOIA_GL_OBJECT(Program);
};

} // namespace render

} // namespace sequoia

#endif
