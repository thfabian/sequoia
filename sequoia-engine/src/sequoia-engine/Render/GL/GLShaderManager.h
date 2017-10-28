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

#ifndef SEQUOIA_RENDER_GL_GLSHADERMANAGER_H
#define SEQUOIA_RENDER_GL_GLSHADERMANAGER_H

#include "sequoia-engine/Core/Assert.h"
#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/Mutex.h"
#include "sequoia-engine/Core/NonCopyable.h"
#include "sequoia-engine/Render/GL/GLShader.h"
#include <memory>
#include <unordered_map>
#include <vector>

namespace sequoia {

namespace render {

/// @brief Manage OpenGL shaders by creating and loading them from source
/// @ingroup gl
class SEQUOIA_API GLShaderManager : public NonCopyable {
  /// Access mutex
  SpinMutex mutex_;

  /// Record of all the registered shaders (use count of 1 implies the shader is *not* in use)
  std::vector<std::shared_ptr<GLShader>> shaderList_;

  /// Lookup map for files
  std::unordered_map<std::shared_ptr<File>, std::size_t> fileLookupMap_;

public:
  /// @brief Destroy all remaining shaders
  ~GLShaderManager();

  /// @brief Create an *empty* shader from source and compile it
  ///
  /// @param type   Type of the shader
  /// @param file   File of the shader source
  /// @returns Newly created shader which is *not* valid, call `Shader::makeValid()` to convert
  ///          it into a valid state
  ///
  /// @remark Thread-safe
  std::shared_ptr<GLShader> create(GLShader::ShaderType type, const std::shared_ptr<File>& file);

  /// @brief Make the shader valid
  /// @throws RenderSystemExcption  Failed to initialize the shader
  void makeValid(GLShader* shader);

  /// @brief Remove the `shader` (do nothing if shader does not exist)
  void remove(const std::shared_ptr<GLShader>& shader) noexcept;
};

} // namespace render

} // namespace sequoia

#endif
