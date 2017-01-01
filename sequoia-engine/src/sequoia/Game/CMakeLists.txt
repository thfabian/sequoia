##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                      S E Q U O I A
##
## This file is distributed under the MIT License (MIT). 
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

sequoia_add_library(
  NAME SequoiaGame
  SOURCES CameraController.cpp
          Game.cpp
          InputManager.cpp
          RenderSystemFactory.cpp
          WindowFactory.cpp
  DEPENDS SequoiaCore
          ${SEQUOIA_OGRE_LIBRARIES}
          ${SEQUOIA_OIS_LIBRARIES}
          ${SEQUOIA_BOOST_LIBRARIES}
          ${SEQUOIA_THREADS_LIBRARIES}
)

