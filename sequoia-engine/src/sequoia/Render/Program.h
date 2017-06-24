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

#ifndef SEQUOIA_RENDER_PROGRAM_H
#define SEQUOIA_RENDER_PROGRAM_H

#include "sequoia/Core/Export.h"
#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Render/RenderSystemObject.h"
#include <memory>
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
class SEQUOIA_API Program : public RenderSystemObject,
                            public NonCopyable,
                            public std::enable_shared_from_this<Program> {
public:
  Program(RenderSystemKind kind);
  virtual ~Program();

  /// @brief Get a log of the program
  virtual std::string getLog() const = 0;

  /// @brief Return the shaders which were linked into this program
  virtual const std::set<std::shared_ptr<Shader>>& getShaders() const = 0;

  /// @brief Convert to string
  virtual std::string toString() const = 0;
};

} // namespace render

} // namespace sequoia

#endif
