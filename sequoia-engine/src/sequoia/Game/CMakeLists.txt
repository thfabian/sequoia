##===------------------------------------------------------------------------------*- CMake -*-===##
##                         _____                        _       
##                        / ____|                      (_)      
##                       | (___   ___  __ _ _   _  ___  _  __ _ 
##                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
##                        ____) |  __/ (_| | |_| | (_) | | (_| |
##                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine
##                                       | |                    
##                                       |_| 
##
## This file is distributed under the MIT License (MIT).
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

sequoia_add_library(
  NAME SequoiaGame
  SOURCES Export.h
          Game.h
          Game.cpp
          Mesh.h
          Mesh.cpp
          MeshManager.h
          MeshManager.cpp
  DEPENDS SequoiaRender
          SequoiaCore
          ${SEQUOIA_EXTERNAL_LIBRARIES}   
)

