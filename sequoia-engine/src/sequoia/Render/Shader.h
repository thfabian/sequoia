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

#ifndef SEQUOIA_RENDER_SHADER_H
#define SEQUOIA_RENDER_SHADER_H

#include "sequoia/Core/Platform.h"
#include "sequoia/Render/Export.h"

namespace sequoia {

/// @brief Shader object
///
/// The shaders are managed by the RenderSystem which takes care of linking them into a program and
/// uploading them to the GPU.
///
/// @see RenderSystem::loadShader
/// @ingroup render
class SEQUOIA_RENDER_API Shader {
public:
  /// @brief Enumerates the types of shaders which can run on the GPU
  enum ShaderType { ST_Vertex, ST_Fragment };

  /// @brief Get the type of the shader
  virtual ShaderType getType() const = 0;

  /// @brief Get the unique identifer of the shader
  virtual unsigned int getID() const = 0;
  operator unsigned int() { return getID(); }

  /// @brief Comparison
  /// @{
  bool operator==(const Shader& other) const { return getID() == other.getID(); }
  bool operator!=(const Shader& other) const { return getID() != other.getID(); }
  /// @}

  /// @brief Get the source path of the shader
  virtual platform::String getSourcePath() const = 0;

  /// @brief Get the source code of the shader
  virtual std::string getSourceCode() const = 0;
};

} // namespace sequoia

#endif
