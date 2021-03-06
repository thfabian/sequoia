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

#ifndef SEQUOIA_ENGINE_RENDER_SHADER_H
#define SEQUOIA_ENGINE_RENDER_SHADER_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/NonCopyable.h"
#include "sequoia-engine/Render/RenderRessource.h"
#include <memory>
#include <string>

namespace sequoia {

namespace render {

/// @brief Shader object
///
/// The shaders are managed by the RenderSystem which takes care of linking them into a program and
/// uploading them to the GPU.
///
/// Note that once a shader is fully assembled (i.e `isValid() == true`), the shader is
/// immutable.
///
/// @see RenderSystem::loadShader
/// @ingroup render
class SEQUOIA_API Shader : public RenderRessource {
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

  Shader(RenderSystemKind kind, ShaderType type);
  virtual ~Shader();

  /// @brief Get the type of the shader
  ShaderType getType() const { return type_; }

  /// @brief Shader type to string
  static const char* shaderTypeToString(ShaderType type);

  /// @brief Get the filename (or path) of the shader source
  virtual const std::string& getFilename() const = 0;

  /// @brief Get the source code of the shader
  virtual const std::string& getSourceCode() const = 0;

  /// @brief Convert to string
  virtual std::string toString() const = 0;

protected:
  virtual void makeValidImpl() override = 0;

private:
  /// Type of the shader
  ShaderType type_;
};

} // namespace render

} // namespace sequoia

#endif
