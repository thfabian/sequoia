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

#ifndef SEQUOIA_RENDER_SHADER_H
#define SEQUOIA_RENDER_SHADER_H

#include "sequoia/Core/File.h"
#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Render/Export.h"
#include "sequoia/Render/RenderSystemObject.h"
#include <memory>

namespace sequoia {

namespace render {

/// @brief Shader object
///
/// The shaders are managed by the RenderSystem which takes care of linking them into a program and
/// uploading them to the GPU.
///
/// @see RenderSystem::loadShader
/// @ingroup render
class SEQUOIA_RENDER_API Shader : public RenderSystemObject,
                                  public NonCopyable,
                                  public std::enable_shared_from_this<Shader> {
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
  ShaderType getType() const;

  /// @brief Get the source file of the shader
  virtual const std::shared_ptr<File>& getFile() const = 0;

  /// @brief Get the source code of the shader
  virtual std::string getSourceCode() const = 0;

  /// @brief Get a log of the shader
  virtual std::string getLog() const = 0;

  /// @brief Shader type to string
  static const char* shaderTypeToString(ShaderType type);

  /// @brief Convert to string
  virtual std::string toString() const = 0;

protected:
  ShaderType type_;
};

} // namespace render

} // namespace sequoia

#endif
