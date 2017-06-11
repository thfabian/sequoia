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
  /// Record of all the registered shaders (use count of 1 implies the shader is *not* in use)
  std::vector<std::shared_ptr<GLShader>> shaderList_;

  /// Lookup map for shader ID
  std::unordered_map<unsigned int, std::size_t> idLookupMap_;

  /// Lookup map for files
  std::unordered_map<std::shared_ptr<File>, std::size_t> fileLookupMap_;

public:
  /// @brief Destroy all remaining shaders
  ~GLShaderManager();

  /// @brief Create the shader from source and compile it
  ///
  /// @param type             Type of the shader
  /// @param file             File of the shader source
  /// @param requestedStatus  Requested target status
  /// @throws RenderSystemException
  std::shared_ptr<GLShader> create(GLShader::ShaderType type, const std::shared_ptr<File>& file,
                                   GLShaderStatus requestedStatus = GLShaderStatus::Compiled);

  /// @brief Convert the shader to `status`
  /// @throws RenderSystemException
  void make(const std::shared_ptr<GLShader>& shader, GLShaderStatus requestedStatus);

  /// @brief Convert the shader to `GLProgramStatus::Linked`
  /// @see GLShaderLoader::make
  void makeValid(const std::shared_ptr<GLShader>& shader) {
    make(shader, GLShaderStatus::Compiled);
  }

  /// @brief Get the shader by OpenGL shader `id`
  const std::shared_ptr<GLShader>& get(unsigned int id) const;
};

} // namespace render

} // namespace sequoia

#endif
