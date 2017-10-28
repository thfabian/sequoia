##===------------------------------------------------------------------------------*- CMake -*-===##
##                         _____                        _
##                        / ____|                      (_)
##                       | (___   ___  __ _ _   _  ___  _  __ _
##                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
##                        ____) |  __/ (_| | |_| | (_) | | (_| |
##                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine (2016-2017)
##                                       | |
##                                       |_|
##
## This file is distributed under the MIT License (MIT).
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

sequoia_add_library(
  NAME SequoiaEngineGame
  SOURCES AssetManager.h
          AssetManager.cpp
          CameraController.cpp
          CameraController.h
          CameraControllerFree.cpp
          CameraControllerFree.h
          Drawable.cpp
          Drawable.h
          Emittable.cpp
          Emittable.h
          Exception.h
          Game.cpp
          Game.h
          GameFwd.h
          Keymap.cpp
          Keymap.h
          Mesh.cpp
          Mesh.h
          MeshManager.cpp
          MeshManager.h
          PointLight.cpp
          PointLight.h
          Scene.cpp
          Scene.h
          SceneGraph.cpp
          SceneGraph.h
          SceneNode.cpp 
          SceneNode.h
          SceneNodeAlloc.h          
          SceneNodeCapability.h
  DEPENDS SequoiaEngineCoreObjects
          SequoiaEngineRenderObjects
  OBJECT
)
