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

#ifndef SEQUOIA_RENDER_DRAWCOMMANDLIST_H
#define SEQUOIA_RENDER_DRAWCOMMANDLIST_H

#include "sequoia/Render/DrawCommand.h"
#include "sequoia/Render/Export.h"
#include <string>
#include <vector>

namespace sequoia {

namespace render {

/// @brief List of draw commands
///
/// The list may sort the draw commands to increase performance by decreasing the changes in the
/// rendering state machine.
///
/// @ingroup render
class SEQUOIA_RENDER_API DrawCommandList {
public:
  /// @brief Insert a `command` into the list
  /// @param command  Command to insert
  virtual void insert(DrawCommand* command) noexcept = 0;

  /// @brief Start iterating the draw commands
  /// @returns first draw command
  virtual DrawCommand* start() noexcept = 0;

  /// @brief Get the next draw command
  /// @returns next draw command or `nullptr` if the end is reached
  virtual DrawCommand* next() noexcept = 0;

  /// @brief Clear the list
  virtual void clear() noexcept = 0;

  /// @brief Convert draw command list to string
  virtual std::string toString() const noexcept = 0;
};

/// @brief Default draw command list without any sorting
/// @ingroup render
class SEQUOIA_RENDER_API DrawCommandListDefault : public DrawCommandList {
  std::vector<DrawCommand*> commands_;
  std::size_t index_;

public:
  DrawCommandListDefault() : DrawCommandList(), index_(0) {}

  /// @copydoc DrawCommandList::insert
  virtual void insert(DrawCommand* command) noexcept override { commands_.push_back(command); }

  /// @copydoc DrawCommandList::start
  virtual DrawCommand* start() noexcept override {
    index_ = 0;
    return commands_.empty() ? nullptr : commands_[index_];
  }

  /// @copydoc DrawCommandList::next
  virtual DrawCommand* next() noexcept override {
    return (SEQUOIA_BUILTIN_UNLIKELY(++index_ == commands_.size()) ? nullptr : commands_[index_]);
  }

  /// @copydoc DrawCommandList::clear
  virtual void clear() noexcept override { commands_.clear(); }

  /// @copydoc DrawCommandList::toString
  std::string toString() const noexcept override;
};

} // namespace render

} // namespace sequoia

#endif
