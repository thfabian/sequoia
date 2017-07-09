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

sequoia_add_test(
  NAME SequoiaGameTest
  SOURCES TestAssetManager.cpp
          TestCameraController.cpp
          TestCameraControllerFree.cpp
          TestDrawable.cpp
          TestKeymap.cpp
          TestMeshManager.cpp
          TestMain.cpp
          TestSceneGraph.cpp
          TestSceneNode.cpp
  DEPENDS Sequoia
          ${SEQUOIA_EXTERNAL_LIBRARIES}
)

