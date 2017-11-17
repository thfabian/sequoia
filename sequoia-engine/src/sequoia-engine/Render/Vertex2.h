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

#ifndef SEQUOIA_ENGINE_RENDER_VERTEX2_H // TODO: remove 2
#define SEQUOIA_ENGINE_RENDER_VERTEX2_H

#include "sequoia-engine/Render/VertexDef.h"

namespace sequoia {

namespace render {

// clang-format off

//===------------------------------------------------------------------------------------------===//
//     Register all vertices below:
//===------------------------------------------------------------------------------------------===//

#define SERUOIA_VERTICES SEQUOIA_REGSTER_VERTICES(                                                 \
  Vertex_posf3_norf3_texf2_colu4,                                                                  \
  Vertex_posf2_texf2_colu4                                                                         \
)

//===------------------------------------------------------------------------------------------===//
//    Define all vertices below:
//===------------------------------------------------------------------------------------------===//

SEQUOIA_DEFINE_VERTEX_ID_ENUM(SERUOIA_VERTICES)

// Standard 3D vertex
SEQUOIA_DEFINE_VERTEX(Vertex_posf3_norf3_texf2_colu4, 
                     (float, Position, 3, false)
                     (float, Normal, 3, false)
                     (float, TexCoord, 2, false)
                     (std::uint8_t, Color, 4, true));

// Standard 2D vertex
SEQUOIA_DEFINE_VERTEX(Vertex_posf2_texf2_colu4, 
                     (float, Position, 2, false)
                     (float, TexCoord, 2, false)
                     (std::uint8_t, Color, 4, true));

// clang-format on

} // namespace render

} // namespace sequoia

#endif
