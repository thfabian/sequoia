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

#ifndef SEQUOIA_ENGINE_RENDER_DRAWSCENE_H
#define SEQUOIA_ENGINE_RENDER_DRAWSCENE_H

#include "sequoia-engine/Core/Export.h"

namespace sequoia {

namespace render {

class SEQUOIA_API DrawScene {
  
  /// @brief Set the uniform struct `name` to `value`
  ///
  /// @param name     Name of the uniform variable
  /// @param value    Struct to upload to the GPU
  /// @param index    If the struct is part of an array, the index of this struct or -1 to indicate
  ///                 this is a scalar struct
  ///
  /// This calls `StructType::toUniformVariableMap` to retrive all the uniform variables of the
  /// struct. The synopsis of `StructType::toUniformVariableMap` is:
  ///
  /// @code{.cpp}
  /// void toUniformVariableMap(
  ///   const std::string& name,
  ///   std::unordered_map<std::string, UniformVariable>& map,
  ///   int index) const;
  /// @endcode
  ///
  /// In general, the struct should be defined via the `SEQUOIA_UNIFORM_STRUCT` macro which
  /// generates the appropriate member function.
//  template <class StructType>
//  void setUniformStruct(const std::string& name, const StructType& value, int index = -1) {
//    value.toUniformVariableMap(name, variables_, index);
//  }
};

} // namespace render

} // namespace sequoia

#endif
