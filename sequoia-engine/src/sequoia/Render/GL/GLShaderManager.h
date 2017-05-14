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

#ifndef SEQUOIA_RENDER_GL_GLSHADERMANAGER_H
#define SEQUOIA_RENDER_GL_GLSHADERMANAGER_H

#include "sequoia/Core/Assert.h"
#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Render/Export.h"
#include "sequoia/Render/GL/GLShader.h"
#include <unordered_map>
#include <vector>

namespace sequoia {

namespace render {

/// @brief Manage OpenGL shaders by creating and loading them from source
///
/// A ShaderManager is attached to a specific OpenGL context.
///
/// @ingroup gl
class SEQUOIA_RENDER_API GLShaderManager : public NonCopyable {
public:
  /// @brief Destroy all remaining shaders
  ~GLShaderManager();
  
  /// @brief Create the shader from source and compile it
  ///
  /// If the shader already exists, its `Status` will be moved to `SK_Compiled`.
  ///
  /// @param type             Type of the shader
  /// @param path             Path to the shader source
  /// @param requestedStatus  Requested target status
  ///
  /// @throws RenderSystemException   Failed to compile shader
  GLShader* create(GLShader::ShaderType type, const platform::String& path,
                   GLShaderStatus requestedStatus = GLShaderStatus::Compiled);

  /// @brief Convert the shader to `status`
  /// @throws RenderSystemException   Failed to compile shader
  void make(GLShader* shader, GLShaderStatus requestedStatus);

  /// @brief Destroy the shader be deregistering it from OpenGL (i.e it's status will be
  /// `GLShaderStatus::InMemory`)
  void destroy(GLShader* shader);

  /// @brief Convert the shader to `SK_Compiled`
  /// @see GLShaderLoader::make
  void makeValid(GLShader* shader) { make(shader, GLShaderStatus::Compiled); }

  /// @brief Get the shader by OpenGL shader `id`
  GLShader* get(unsigned int id) const;

private:
  /// Record of all the registered and compiled shaders
  std::vector<std::unique_ptr<GLShader>> shaderList_;

  /// Lookup map for shader ID
  std::unordered_map<unsigned int, std::size_t> idLookupMap_;

  /// Lookup map for path
  std::unordered_map<platform::String, std::size_t> pathLookupMap_;
};

} // namespace render

} // namespace sequoia

#endif
