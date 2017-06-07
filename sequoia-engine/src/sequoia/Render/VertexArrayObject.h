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

#ifndef SEQUOIA_RENDER_VERTEXARRAYOBJECT_H
#define SEQUOIA_RENDER_VERTEXARRAYOBJECT_H

#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Render/Export.h"
#include "sequoia/Render/RenderSystemObject.h"
#include "sequoia/Render/Vertex.h"
#include <memory>

namespace sequoia {

namespace render {

class VertexData;

/// @brief Handling of layout and buffering of an array of vertices
///
/// @see RenderSystem::createVertexArrayObject
/// @ingroup render
class SEQUOIA_RENDER_API VertexArrayObject : public RenderSystemObject, public NonCopyable {
public:
  /// @brief Usage hint of the vertex buffer
  enum BufferUsageKind {
    BK_Invalid = 0,

    ///< Static buffer which the application rarely modifies once created. Modifying the contents of
    ///  this buffer will involve a performance hit.
    BK_Static = 1,

    ///< Indicates the application would like to modify this buffer with the CPU fairly often.
    ///  This is the least optimal buffer setting.
    BK_Dynamic = 2,

    ///< Indicates the application will never read the contents of the buffer back,
    ///  it will only ever write data.
    BK_WriteOnly = 4,

    ///< Indicates that the application will be refilling the contents of the buffer regularly (not
    ///  just updating, but generating the contents from scratch), and therefore does not mind if
    ///  the contents of the buffer are lost somehow and need to be recreated. This allows and
    ///  additional level of optimisation on the buffer.
    BK_Discardable = 8,

    ///< Combination of `BK_Static` and `BK_WriteOnly`. This is the optimal buffer usage setting.
    BK_StaticWriteOnly = 5,

    ///< Combination of `BK_Dynamic` and `BK_WriteOnly`.
    BK_DynamicWriteOnly = 6,

    ///< Combination of `BK_Dynamic`, `BK_WriteOnly` and `BK_Discardable`. This means that you
    ///  expect to replace the entire contents of the buffer on an extremely regular basis, most
    ///  likely every frame. By selecting this option, you free the system up from having to be
    ///  concerned about losing the existing contents of the buffer at any time, because if it does
    ///  lose them, you will be replacing them next frame anyway.
    BK_DynamicWriteOnlyDiscardable = 14
  };

  virtual ~VertexArrayObject();
  VertexArrayObject(RenderSystemKind kind);

  /// @brief Attach `vertexData` and store it in a hardware buffer
  ///
  /// This will create the underlying device hardware buffers. However, the data is only copied to
  /// the GPU once `updateGPU` is called.
  void attachVertexData(VertexData* data, BufferUsageKind usage);

  /// @brief Free all allocated buffers
  void freeVertexData();

  /// @brief Update the vertex data on the device to match the data pointer to by `dataPtr`
  ///
  /// @param offset   Offset to start writing
  /// @param length   Number of vertices to write in `[0, getNumVertices())`
  virtual void writeVertexData(std::size_t offset, std::size_t length) = 0;

  /// @brief Update the index data on the device to match the data pointer to by `dataPtr`
  ///
  /// @param offset   Offset to start writing
  /// @param length   Number of vertices to write  in `[0, getNumIndices())`
  virtual void writeIndexData(std::size_t offset, std::size_t length) = 0;

  /// @brief Convert to string
  virtual std::string toString() const = 0;

  /// @brief Get the number of vertices
  std::size_t getNumVertices() const;

  /// @brief Get the number of indices
  std::size_t getNumIndices() const;

  /// @brief Do we draw with indices?
  bool hasIndices() const;
  
  /// @brief Get the underlying VertexData
  VertexData* getVertexData();

protected:
  /// @brief Allocate vertex buffers on the GPU
  virtual void attachVertexDataDevice() = 0;

  /// @brief Free vertices on the GPU
  virtual void freeVertexDataDevice() = 0;

protected:
  /// VertexData
  VertexData* data_;

  /// Usage of the buffer
  BufferUsageKind usage_;
};

} // namespace render

} // namespace sequoia

#endif
