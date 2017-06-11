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

#ifndef SEQUOIA_GAME_MESH_H
#define SEQUOIA_GAME_MESH_H

#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Game/Export.h"
#include "sequoia/Render/VertexData.h"
#include <memory>

namespace sequoia {

namespace game {

class MeshManager;

/// @brief Ressource holding the 3D mesh
/// @ingroup game
class SEQUOIA_GAME_API Mesh : public NonCopyable {
public:
  friend class MeshManager;

  /// @brief Create empty mesh
  Mesh(const std::string& name, const std::shared_ptr<render::VertexData>& data, bool isModifiable);

  /// @brief Get the local bounding box
  const math::AxisAlignedBox& getAxisAlignedBox() const { return data_->getAxisAlignedBox(); }

  /// @brief Get the associated VertexArrayObject
  render::VertexArrayObject* getVertexArrayObject() const { return data_->getVertexArrayObject(); }

  /// @brief Get the VertexData
  const render::VertexData* getVertexData() { return data_.get(); }
  render::VertexData* getVertexData() const { return data_.get(); }

  /// @brief Accept a VertexVisitor to access/modify the underlying vertex data
  void accept(render::VertexVisitor& visitor) const;

  /// @brief Obtain the vertex data for reading/writing
  /// @see render::VertexData::write
  /// @endcode
  /// @{
  template <class FunctorType>
  void write(FunctorType&& functor) const {
    if(!isModifiable_)
      logWarning();
    else
      data_->write(std::forward<FunctorType>(functor));
  }
  template <class FunctorType>
  void read(FunctorType&& functor) const {
    data_->read(std::forward<FunctorType>(functor));
  }
  /// @}

  /// @brief Set the name of the mesh
  void setName(const std::string& name) { name_ = name; }

  /// @brief Get the name of the mesh
  const std::string& getName() const { return name_; }

  /// @brief Is the mesh modifyable?
  bool isModifiable() const { return isModifiable_; }

  /// @brief Dump the vertex data and indices to `stdout`
  /// @see render::VertexData::dump
  void dump() const;

  /// @brief Convert to string
  std::string toString() const;

private:
  void logWarning() const;

private:
  /// Do we own the mesh i.e can we modify it?
  const bool isModifiable_;

  /// Vertex data
  std::shared_ptr<render::VertexData> data_;

  /// Name of the mesh
  std::string name_;
};

} // namespace game

} // namespace sequoia

#endif
