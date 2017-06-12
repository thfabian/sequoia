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

#ifndef SEQUOIA_RENDER_DRAWCOMMANDLIST_H
#define SEQUOIA_RENDER_DRAWCOMMANDLIST_H

#include "sequoia/Core/AlignedADT.h"
#include "sequoia/Core/ArrayRef.h"
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
protected:
  /// List of commands
  core::aligned_vector<DrawCommand> commands_;

public:
  /// @brief Default size of a DrawCommandList
  static constexpr std::size_t DefaultSize = 1024;

  DrawCommandList() { commands_.reserve(DefaultSize); }

  /// @brief Copy `commands` into the list of DrawCommands (after this operation the `commands` can
  /// be modified again)
  /// @param commands   DrawCommands to insert
  void insert(ArrayRef<DrawCommand*> commands) noexcept {
    for(std::size_t i = 0; i < commands.size(); ++i)
      commands_.push_back(*commands[i]);
  }

  /// @brief Start iterating the draw commands
  /// @returns first draw command
  virtual DrawCommand* start() noexcept = 0;

  /// @brief Get the next draw command
  /// @returns next draw command or `nullptr` if the end is reached
  virtual DrawCommand* next() noexcept = 0;

  /// @brief Clear the list
  void clear() noexcept { commands_.clear(); }

  /// @brief Convert draw command list to string
  virtual std::string toString() const noexcept = 0;
};

/// @brief Default draw command list without any sorting
/// @ingroup render
class SEQUOIA_RENDER_API DrawCommandListDefault : public DrawCommandList {
  std::size_t index_;

public:
  DrawCommandListDefault() : DrawCommandList(), index_(0) {}

  /// @copydoc DrawCommandList::start
  virtual DrawCommand* start() noexcept override {
    return commands_.empty() ? nullptr : &commands_[index_ = 0];
  }

  /// @copydoc DrawCommandList::next
  virtual DrawCommand* next() noexcept override {
    return (SEQUOIA_BUILTIN_UNLIKELY(++index_ == commands_.size()) ? nullptr : &commands_[index_]);
  }

  /// @copydoc DrawCommandList::toString
  std::string toString() const noexcept override;
};

} // namespace render

} // namespace sequoia

#endif
