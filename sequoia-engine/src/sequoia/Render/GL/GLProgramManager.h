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

#ifndef SEQUOIA_RENDER_GL_GLPROGRAMMANAGER_H
#define SEQUOIA_RENDER_GL_GLPROGRAMMANAGER_H

#include "sequoia/Core/Export.h"
#include "sequoia/Core/Mutex.h"
#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Render/GL/GLProgram.h"
#include <memory>
#include <unordered_map>
#include <vector>

namespace sequoia {

namespace render {

/// @brief Create and compile OpenGL GPU programs
/// @ingroup gl
class SEQUOIA_API GLProgramManager : public NonCopyable {
  /// Access mutex
  SpinMutex mutex_;

  /// Record of all the registered programs (use count of 1 implies the program is *not* in use)
  std::vector<std::shared_ptr<GLProgram>> programList_;

  /// Lookup map for programs (hash of the shader set to index in programList)
  std::unordered_map<std::size_t, std::size_t> shaderSetLookupMap_;

public:
  /// @brief Destroy all remaining programs
  ~GLProgramManager();

  /// @brief Create an *empty* program using `shaders`
  ///
  /// The `shaders` need to be valid at this point.
  ///
  /// @param shaders   Shaders linked into the program
  /// @returns Newly created program which is *not* valid, call Program::makeValid() to convert
  ///          it into a valid state
  ///
  /// @remark Thread-safe
  std::shared_ptr<GLProgram> create(const std::set<std::shared_ptr<Shader>>& shaders);

  /// @brief Make the program valid
  /// @throws RenderSystemExcption  Failed to initialize the program
  void makeValid(GLProgram* program);

  /// @brief Remove the `program` (do nothing if the program does not exist)
  void remove(const std::shared_ptr<GLProgram>& program) noexcept;

  /// @brief Compute hash of the set of shaders
  static std::size_t hash(const std::set<std::shared_ptr<Shader>>& shaders) noexcept;

private:
  /// @brief Query the uniform variables of the program
  void getUniforms(GLProgram* program) const;

  /// @brief Set the `location` of all the known vertex attributes (called pre-link)
  void setVertexAttributes(GLProgram* program) const;

  /// @brief Check all the active attributes (called post-link)
  bool checkVertexAttributes(GLProgram* program) const;

  /// @brief Set the `location` of all the known fragment data (called pre-link)
  void setFragmentData(GLProgram* program) const;

  /// @brief Check all the active fragment data (called post-link)
  bool checkFragmentData(GLProgram* program) const;
};

} // namespace render

} // namespace sequoia

#endif
