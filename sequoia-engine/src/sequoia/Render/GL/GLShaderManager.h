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
#include "sequoia/Core/EnumState.h"
#include "sequoia/Render/Export.h"
#include "sequoia/Render/GL/GLShader.h"
#include <map>
#include <unordered_map>

namespace sequoia {

namespace render {

/// @brief Manage OpenGL shaders by creating and loading them from source
///
/// A ShaderManager is attached to a specific OpenGL context.
///
/// @ingroup gl
class SEQUOIA_RENDER_API GLShaderManager {
public:
  enum class ShaderStatus { OnDisk = 0, InMemory, Created, Compiled };
  using ShaderStatusState =
      core::EnumState<ShaderStatus, ShaderStatus::OnDisk, ShaderStatus::InMemory,
                      ShaderStatus::Created, ShaderStatus::Compiled>;

  friend ShaderStatus operator++(ShaderStatus& s, int) {
    ShaderStatus sold = s;
    ShaderStatusState::advance(s);
    return sold;
  }

  //
  // TODO: !!! ShaderRecord should be Shader itself !!!
  //

  /// @brief Useful state information of a OpenGL shader
  struct ShaderRecord {
    ShaderRecord() : Status(ShaderStatus::OnDisk), Shader(nullptr) {}

    /// Status of the shader
    ShaderStatus Status;

    /// OpenGL shader
    std::unique_ptr<GLShader> Shader;

    /// Source code of the shader
    std::string Code;

    /// Source path the shader was loaded from
    platform::String Path;
  };

  /// @brief Get the shader by `id`
  GLShader* get(unsigned int id) const {
    auto it = shaderRecordMap_.find(id);
    SEQUOIA_ASSERT_MSG(it != shaderRecordMap_.end(), "invalid shader id");
    return it->second->Shader.get();
  }

  /// @brief Create the shader from source and compile it
  ///
  /// If the shader already exists, its `Status` will be moved to `SK_Compiled`.
  ///
  /// @throws RenderSystemException   Failed to compile shader
  GLShader* create(const platform::String& path);

private:
  /// @brief Convert the status of the shader to `SK_Compiled` if possible
  /// @returns the id of the shader
  GLShader* makeValid(std::unique_ptr<ShaderRecord>& record);

private:
  /// Record of all the registered and compiled shaders
  std::unordered_map<unsigned int, std::unique_ptr<ShaderRecord>> shaderRecordMap_;

  /// Lookup map for path to ID
  std::unordered_map<platform::String, unsigned int> pathLookupMap_;
};

} // namespace render

} // namespace sequoia

#endif
