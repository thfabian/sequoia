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

#ifndef SEQUOIA_ENGINE_RENDER_VERTEXFACTORY_H
#define SEQUOIA_ENGINE_RENDER_VERTEXFACTORY_H

#include "sequoia-engine/Render/Exception.h"
#include "sequoia-engine/Render/VertexAdapter.h"
#include <boost/preprocessor/stringize.hpp>
#include <string>

namespace sequoia {

namespace render {

#define SEQUOIA_PP_VF_MAKE_LAYOUT(r, Data, Vertex)                                                 \
  if(identifier == BOOST_PP_STRINGIZE(Vertex))                                                     \
    return VertexAdapter(Vertex::getLayout());

/// @brief Create vertices using a string identifier
/// @ingroup render
class SEQUOIA_API VertexFactory {
public:
  VertexFactory() = delete;

  /// @brief Create a layout given by `identifier`
  ///
  /// @param identifier     Type name of the Vertex
  /// @throws RenderSystemException   unknown vertex identifier
  static VertexAdapter create(const std::string& identifier) {
    BOOST_PP_SEQ_FOR_EACH(SEQUOIA_PP_VF_MAKE_LAYOUT, Data, SERUOIA_VERTICES);
    SEQUOIA_THROW(RenderSystemException, "unknown vertex identifier '{}'", identifier);
    return VertexAdapter(VertexLayout2{});
  }
};

} // namespace render

} // namespace sequoia

#endif
