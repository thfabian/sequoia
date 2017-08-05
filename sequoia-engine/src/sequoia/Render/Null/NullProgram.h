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

#ifndef SEQUOIA_RENDER_NULL_NULLPROGRAM_H
#define SEQUOIA_RENDER_NULL_NULLPROGRAM_H

#include "sequoia/Render/Program.h"

namespace sequoia {

namespace render {

/// @brief Null program implementation
/// @ingroup null
class SEQUOIA_API NullProgram final : public Program {
public:
  NullProgram(const std::set<std::shared_ptr<Shader>>& shaders);
  virtual ~NullProgram();

  /// @copydoc Program::getShaders
  virtual const std::set<std::shared_ptr<Shader>>& getShaders() const override;

  /// @copydoc Program::toString
  virtual std::string toString() const override;

  SEQUOIA_GL_OBJECT(Program)

protected:
  virtual void makeValidImpl() override;

private:
  std::set<std::shared_ptr<Shader>> shaders_;
};

} // namespace render

} // namespace sequoia

#endif
