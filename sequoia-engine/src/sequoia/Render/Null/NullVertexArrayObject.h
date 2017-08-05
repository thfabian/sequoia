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

#ifndef SEQUOIA_RENDER_NULL_NULLVERTEXARRAYOBJECT_H
#define SEQUOIA_RENDER_NULL_NULLVERTEXARRAYOBJECT_H

#include "sequoia/Render/VertexArrayObject.h"

namespace sequoia {

namespace render {

/// @brief Null vertex array
/// @ingroup null
class SEQUOIA_API NullVertexArrayObject final : public VertexArrayObject {
public:
  virtual ~NullVertexArrayObject();
  NullVertexArrayObject();

  /// @copydoc VertexArrayObject::updateVertexData
  virtual void writeVertexData(std::size_t offset, std::size_t length) override;

  /// @copydoc VertexArrayObject::updateIndexData
  virtual void writeIndexData(std::size_t offset, std::size_t length) override;

  /// @copydoc VertexArrayObject::toString
  virtual std::string toString() const override;

  SEQUOIA_GL_OBJECT(VertexArrayObject);

protected:
  /// @copydoc VertexArrayObject::attachVertexDataDevice
  virtual void attachVertexDataDevice() override;

  /// @copydoc VertexArrayObject::freeVertexDataDevice
  virtual void freeVertexDataDevice() override;
};

} // namespace render

} // namespace sequoia

#endif
