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

#ifndef SEQUOIA_RENDER_PROGRAM_H
#define SEQUOIA_RENDER_PROGRAM_H

#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Render/Export.h"
#include "sequoia/Render/RenderSystemObject.h"
#include <set>
#include <string>

namespace sequoia {

namespace render {

class Shader;

/// @brief A high-level GPU program
///
/// A GPU program consist of multiple shaders which are linked together in a program which can be
/// used during the rendering stage to control various aspects of the rendering pipeline.
///
/// @see RenderSystem::createPogram
/// @ingroup render
class SEQUOIA_RENDER_API Program : public RenderSystemObject, public NonCopyable {
public:
  virtual ~Program();
  Program(RenderSystemKind kind);

  /// @brief Get the unique identifer of the program
  ///
  /// Note that IDs might be reused after a program has been destroyed.
  virtual unsigned int getID() const = 0;
  operator unsigned int() { return getID(); }

  /// @brief Comparison
  /// @{
  bool operator==(const Program& other) const { return getID() == other.getID(); }
  bool operator!=(const Program& other) const { return getID() != other.getID(); }
  /// @}

  /// @brief Convert program to a human-readable string representation
  virtual std::string toString() const = 0;

  /// @brief Return the shaders which were linked into this program
  virtual const std::set<Shader*>& getShaders() const = 0;
};

} // namespace render

} // namespace sequoia

#endif
