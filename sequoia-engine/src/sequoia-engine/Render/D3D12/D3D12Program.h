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

#ifndef SEQUOIA_ENGINE_RENDER_D3D12_D3D12PROGRAM_H
#define SEQUOIA_ENGINE_RENDER_D3D12_D3D12PROGRAM_H

#include "sequoia-engine/Render/Program.h"

namespace sequoia {

namespace render {

/// @brief D3D12 program implementation
/// @ingroup d3d12
class SEQUOIA_API D3D12Program final : public Program {
public:
  D3D12Program(const std::set<std::shared_ptr<Shader>>& shaders);
  virtual ~D3D12Program();

  /// @copydoc Program::getShaders
  virtual const std::set<std::shared_ptr<Shader>>& getShaders() const override;

  /// @copydoc Program::toString
  virtual std::string toString() const override;

  SEQUOIA_D3D12_OBJECT(Program)

protected:
  virtual void makeValidImpl() override;

private:
  std::set<std::shared_ptr<Shader>> shaders_;
};

} // namespace render

} // namespace sequoia

#endif
