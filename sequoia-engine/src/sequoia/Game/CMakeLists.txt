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
  NAME SequoiaGame
  SOURCES AssetManager.h
          AssetManager.cpp
          CameraController.h
          CameraController.cpp
          CameraControllerFree.h
          CameraControllerFree.cpp
          Drawable.h
          Drawable.cpp
          Export.h
          Game.h
          Game.cpp
          Mesh.h
          Mesh.cpp
          MeshManager.h
          MeshManager.cpp
          Keymap.h
          Keymap.cpp
          Scene.h
          Scene.cpp
          SceneGraph.h
          SceneGraph.cpp
          SceneNode.h
          SceneNode.cpp
  DEPENDS SequoiaRender
          SequoiaCore
          ${SEQUOIA_EXTERNAL_LIBRARIES}   
)

