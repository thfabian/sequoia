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

#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Core/Platform.h"
#include "sequoia/Render/Export.h"
#include "sequoia/Render/RenderSystemObject.h"

namespace sequoia {

namespace render {

/// @brief Shader object
///
/// The shaders are managed by the RenderSystem which takes care of linking them into a program and
/// uploading them to the GPU.
///
/// @see RenderSystem::loadShader
/// @ingroup render
class SEQUOIA_RENDER_API Shader : public RenderSystemObject, public NonCopyable {
public:
  /// @brief Enumerates the types of shaders which can run on the GPU
  enum ShaderType {
    ST_Compute,
    ST_Vertex,
    ST_TessControl,
    ST_TessEvaluation,
    ST_Geometry,
    ST_Fragment
  };

  virtual ~Shader();
  Shader(RenderSystemKind kind, ShaderType type);

  /// @brief Get the type of the shader
  ShaderType getType() const;

  /// @brief Get the source path of the shader
  virtual platform::String getSourcePath() const = 0;

  /// @brief Get the source code of the shader
  virtual std::string getSourceCode() const = 0;

  /// @brief Convert program to a human-readable string representation
  virtual std::string toString() const = 0;

  /// @brief Shader type to string
  static const char* shaderTypeToString(ShaderType type);

protected:
  ShaderType type_;
};

} // namespace render

} // namespace sequoia

#endif
