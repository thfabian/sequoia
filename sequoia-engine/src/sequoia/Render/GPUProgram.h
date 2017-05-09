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

#ifndef SEQUOIA_RENDER_GPUPROGRAM_H
#define SEQUOIA_RENDER_GPUPROGRAM_H

#include "sequoia/Render/Export.h"
#include <vector>

namespace sequoia {

class Shader;

/// @brief High-level GPU program
///
/// A GPU program consist of multiple shaders which are linked together in program which can be used
/// during the rendering stage to control various aspects of the rendering pipeline.
///
/// @see RenderSystem::createGPUPogram
class SEQUOIA_RENDER_API GPUProgram {
public:
  /// @brief Get the unique identifer of the program
  virtual unsigned int getID() const = 0;
  operator unsigned int() { return getID(); }

  /// @brief Return the shaders which were linked into this program
  virtual std::vector<Shader*> getShaders() const = 0;
};

} // namespace sequoia

#endif
