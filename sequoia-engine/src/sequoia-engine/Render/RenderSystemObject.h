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

#ifndef SEQUOIA_ENGINE_RENDER_RENDERSYSTEMOBJECT_H
#define SEQUOIA_ENGINE_RENDER_RENDERSYSTEMOBJECT_H

namespace sequoia {

namespace render {

/// @brief Enumeration of all RenderSystems
/// @ingroup render
enum RenderSystemKind {
  RK_Invalid = 0,
  RK_Null,  ///< Null implementation
  RK_OpenGL ///< OpenGL implementation
};

/// @brief Base class of all RenderSystem specific objects to provide fast RTTI
/// @ingroup render
class RenderSystemObject {
public:
  RenderSystemObject(RenderSystemKind renderSystemKind) : renderSystemKind_(renderSystemKind) {}

  /// @brief Get the kind of render-system this object was registered with
  RenderSystemKind getRenderSystemKind() const noexcept { return renderSystemKind_; }

protected:
  RenderSystemKind renderSystemKind_;
};

#define SEQUOIA_OBJECT_IMPL(Type, Kind)                                                            \
  inline static bool classof(const Type* type) noexcept {                                          \
    return type->getRenderSystemKind() == Kind;                                                    \
  }

/// @brief Declare as OpenGL RenderSystemObject
/// @ingroup gl
#define SEQUOIA_GL_OBJECT(Type) SEQUOIA_OBJECT_IMPL(Type, sequoia::render::RK_OpenGL)

/// @brief Declare as Null RenderSystemObject
/// @ingroup null
#define SEQUOIA_NULL_OBJECT(Type) SEQUOIA_OBJECT_IMPL(Type, sequoia::render::RK_Null)

} // namespace render

} // namespace sequoia

#endif